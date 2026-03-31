#include "pico/bootrom.h"
#include "lib/pico_rp2350.h"
#include "lib/lcd_st7735s.h"
#include "wshare_lcd/LCD_1in8.h"
#include "lib/ublox_neo6m.h"
#include "lib/bk_dht22.h"
#include "pico/multicore.h"

#define DEBUG_ENV_RESET_TIMEOUT (60 * 1000)

void do_temperature(dht22_data_t *dht22_data);
void do_gps(void);

void core1_entry() {
    while (true) {
        do_gps();
    }
}

int main(void) {
    pico_rp2350_init();
    // add_alarm_in_ms(DEBUG_ENV_RESET_TIMEOUT, pico_rp2350_reset, NULL, false);

    // Run GPS updates on Cortex-M core 1 while main is running on core 0
    multicore_launch_core1(core1_entry);

    dht22_data_t dht22_data;
    while (true) {
        do_temperature(&dht22_data);
        sleep_ms(2000); // Minimum 2s between reads
    }
}

void do_temperature(dht22_data_t *dht22_data) {
    if (dht22_read(dht22_data)) {
        printf("Temperature: %.1f C, Humidity: %.1f %%\n", dht22_data->temperature, dht22_data->humidity);
        char temp_str[16], hum_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.1fC", dht22_data->temperature);
        snprintf(hum_str, sizeof(hum_str), "%.1f%%", dht22_data->humidity);

        lcd_st7735s_lock();
        lcd_st7735s_set_line(temp_str, 0, BLACK);
        lcd_st7735s_set_line(hum_str, 1, BLACK);
        lcd_st7735s_draw_lines();
        lcd_st7735s_unlock();
    } else {
        printf("DHT22 read failed\n");
    }
}

void do_gps() {
    if (ublox_neo6m_read_next_nmea_sentence()) {
        ublox_neo6m_parse_nmea_sentence();

        const char *timestamp = ublox_neo6m_get_timestamp();
        const char *status = ublox_neo6m_get_status();

        lcd_st7735s_lock();
        lcd_st7735s_set_line(timestamp, 2, BLACK);
        if (strcmp(status, "A") == 0) {
            const char *speed_kmh = ublox_neo6m_get_speed_kmh();
            lcd_st7735s_set_line(speed_kmh, 3, BLUE);
            lcd_st7735s_set_line("", 4, BLACK);
            lcd_st7735s_set_line("", 5, BLACK);
        } else {
            lcd_st7735s_set_line("", 3, BLACK);
            lcd_st7735s_set_line("GPS init..", 4, RED);
            lcd_st7735s_set_line("", 5, BLACK);
        }
        lcd_st7735s_draw_lines();
        lcd_st7735s_unlock();
    }
}
