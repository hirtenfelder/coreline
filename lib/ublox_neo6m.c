#include "ublox_neo6m.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/gpio.h"

#define UART_ID uart1
#define BAUD_RATE 9600
#define UART_TX_PIN 4 // Pico TX -> GPS RX
#define UART_RX_PIN 5 // Pico RX <- GPS TX
#define NMEA_BUF_SIZE 128

static char nmea[NMEA_BUF_SIZE];
static struct gps_data nmea_data;

int ublox_neo6m_init() {
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    return EXIT_SUCCESS;
}

bool ublox_neo6m_read_next_nmea_sentence() {
    static size_t idx = 0;
    static bool collecting = false;

    while (uart_is_readable(UART_ID)) {
        char c = uart_getc(UART_ID);

        if (c == '$') {
            collecting = true;
            idx = 0;
            nmea[idx++] = c;
            continue;
        }
        if (!collecting || c == '\r') {
            continue;
        }
        if (c == '\n') {
            nmea[idx] = '\0';
            collecting = false;
            printf("Got NMEA: %s\n", nmea);
            return true; // one full sentence ready
        }
        if (idx < NMEA_BUF_SIZE - 1) {
            nmea[idx++] = c;
        } else {
            // overflow: drop this sentence
            collecting = false;
            idx = 0;
        }
    }
    return false; // no complete sentence yet
}

void ublox_neo6m_parse_nmea_sentence() {
    if (strncmp(nmea, "$GPRMC", 6) == 0) {
        char *p = nmea;

        // Find first comma
        p = strchr(p, ',');
        if (p != NULL) {
            p++; // move past the comma
            // Copy up to 6 characters for HHMMSS
            for (int i = 0; i < 6; i++) {
                nmea_data.gprmc_timestamp[i] = p[i];
            }
            nmea_data.gprmc_timestamp[6] = '\0';
        }
        // Find next comma
        p = strchr(p, ',');
        if (p != NULL) {
            p++;
            nmea_data.gprmc_status = p[0] == ',' ? '?' : p[0];
        }

        printf("Got timestamp: %s\n", nmea_data.gprmc_timestamp);
        printf("Got status: %c\n", nmea_data.gprmc_status);
    }
}

char *ublox_neo6m_get_timestamp() {
    // Format "HH:mm:ss"
    static char formatted[9];
    // Expecting "HHMMSS"
    const char *ts = nmea_data.gprmc_timestamp;

    // Require at least 6 digits available
    for (int i = 0; i < 6; i++) {
        if (ts[i] < '0' || ts[i] > '9') {
            return "--:--:--";
        }
    }

    formatted[0] = ts[0];
    formatted[1] = ts[1];
    formatted[2] = ':';
    formatted[3] = ts[2];
    formatted[4] = ts[3];
    formatted[5] = ':';
    formatted[6] = ts[4];
    formatted[7] = ts[5];
    formatted[8] = '\0';

    return formatted;
}

char *ublox_neo6m_get_status() {
    return &nmea_data.gprmc_status;
}
