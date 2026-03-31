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

static const char *collect_nmea_value(const char *p, char *value, const size_t value_size) {
    assert(value != NULL);
    assert(value_size >= 2);
    if (!p) {
        return NULL;
    }

    // Find next comma
    p = strchr(p, ',');
    if (!p) {
        return NULL;
    }
    p++;

    // Collect all characters until next comma or end marker
    size_t i = 0;
    while (p[i] != ',' && p[i] != '\0' && p[i] != '*' && i < value_size - 1) {
        value[i] = p[i];
        i++;
    }

    // In case of empty value, use question mark as value.
    if (i == 0) {
        value[i++] = '?';
    }

    // Add end marker
    value[i] = '\0';

    return p + i;
}

void print_nmea() {
    printf("-> timestamp: %s\n", nmea_data.gprmc_timestamp);
    printf("-> status: %s\n", nmea_data.gprmc_status);
    printf("-> latitude: %s\n", nmea_data.gprmc_latitude);
    printf("-> latitude direction: %s\n", nmea_data.gprmc_latitude_direction);
    printf("-> longitude: %s\n", nmea_data.gprmc_longitude);
    printf("-> longitude direction: %s\n", nmea_data.gprmc_longitude_direction);
    printf("-> speed knots: %s\n", nmea_data.gprmc_speed_knots);
    printf("-> course over ground: %s\n", nmea_data.gprmc_course_over_ground);
    printf("-> date: %s\n", nmea_data.gprmc_date);
}

void ublox_neo6m_parse_nmea_sentence() {
    printf("Got NMEA sentence %s \n", nmea);

    // Example GPRMC NMEA:
    // $GPRMC,211124.00,V,,,,,,,260326,,,N*7B
    // $GPRMC,040302.663,A,3939.7,N,10506.6,W,0.27,358.86,200804,,*1A

    if (strncmp(nmea, "$GPRMC", 6) == 0) {
        const char *p = nmea;

        p = collect_nmea_value(p, nmea_data.gprmc_timestamp, sizeof(nmea_data.gprmc_timestamp));
        p = collect_nmea_value(p, nmea_data.gprmc_status, sizeof(nmea_data.gprmc_status));
        p = collect_nmea_value(p, nmea_data.gprmc_latitude, sizeof(nmea_data.gprmc_latitude));
        p = collect_nmea_value(p, nmea_data.gprmc_latitude_direction, sizeof(nmea_data.gprmc_latitude_direction));
        p = collect_nmea_value(p, nmea_data.gprmc_longitude, sizeof(nmea_data.gprmc_longitude));
        p = collect_nmea_value(p, nmea_data.gprmc_longitude_direction, sizeof(nmea_data.gprmc_longitude_direction));
        p = collect_nmea_value(p, nmea_data.gprmc_speed_knots, sizeof(nmea_data.gprmc_speed_knots));
        p = collect_nmea_value(p, nmea_data.gprmc_course_over_ground, sizeof(nmea_data.gprmc_course_over_ground));
        p = collect_nmea_value(p, nmea_data.gprmc_date, sizeof(nmea_data.gprmc_date));

        if (!p) {
            printf("Failed to parse GPRMC fields.\n");
            return;
        }

        // print_nmea();
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

char* ublox_neo6m_get_date() {
    // Format "dd.MM.yyyy"
    static char formatted[11];
    const char *ts = nmea_data.gprmc_date;

    // Require at least 6 digits available
    for (int i = 0; i < 6; i++) {
        if (ts[i] < '0' || ts[i] > '9') {
            return "--.--.----";
        }
    }

    formatted[0] = ts[0];
    formatted[1] = ts[1];
    formatted[2] = '.';
    formatted[3] = ts[2];
    formatted[4] = ts[3];
    formatted[5] = '.';
    formatted[6] = '2';
    formatted[7] = '0';
    formatted[8] = ts[4];
    formatted[9] = ts[5];
    formatted[10] = '\0';

    return formatted;
}

char *ublox_neo6m_get_status() {
    return nmea_data.gprmc_status;
}

char *ublox_neo6m_get_speed_kmh() {
    static char formatted[12];

    if (nmea_data.gprmc_speed_knots[0] == '\0' || nmea_data.gprmc_speed_knots[0] == '?') {
        formatted[0] = '0';
        formatted[1] = '\0';
        return formatted;
    }

    const float knots = strtof(nmea_data.gprmc_speed_knots, NULL);
    const float kmh = knots * 1.852f;

    snprintf(formatted, sizeof(formatted), "%.2f", kmh);
    return formatted;
}
