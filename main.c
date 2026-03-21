#include "pico/bootrom.h"
#include "lib/pico_rp2350.h"
#include "lib/lcd_st7735s.h"
#include "wshare_lcd/LCD_1in8.h"
#include "lib/ublox_neo6m.h"
#include "lib/bk_dht22.h"

#define DEBUG_ENV_RESET_TIMEOUT (60 * 1000)

void do_temperature(dht22_data_t dht22_data);
void do_gps(void);

int main(void) {
    pico_rp2350_init();
    // add_alarm_in_ms(DEBUG_ENV_RESET_TIMEOUT, pico_rp2350_reset, NULL, false);

    dht22_data_t dht22_data;
    while (true) {
        do_temperature(dht22_data);
        sleep_ms(2000); // Minimum 2s between reads
        do_gps();
    }
}

void do_temperature(dht22_data_t dht22_data) {
    if (dht22_read(&dht22_data)) {
        printf("Temperature: %.1f C, Humidity: %.1f %%\n", dht22_data.temperature, dht22_data.humidity);
        char temp_str[16], hum_str[16];
        snprintf(temp_str, sizeof(temp_str), "%.1fC", dht22_data.temperature);
        snprintf(hum_str, sizeof(hum_str), "%.1f%%", dht22_data.humidity);
        lcd_st7735s_set_line(temp_str, 0, BLACK);
        lcd_st7735s_set_line(hum_str, 1, BLACK);
        lcd_st7735s_draw_lines();
    } else {
        printf("DHT22 read failed\n");
    }
}

void do_gps() {
    if (ublox_neo6m_read_next_nmea_sentence()) {
        ublox_neo6m_parse_nmea_sentence();
        const char *timestamp = ublox_neo6m_get_timestamp();
        const char *status = ublox_neo6m_get_status();
        lcd_st7735s_set_line(timestamp, 2, BLACK);
        lcd_st7735s_set_line(status, 3, BLACK);
        lcd_st7735s_draw_lines();
        DEV_Delay_ms(1000);
        if (strcmp(status, "A") != 0) {
            lcd_st7735s_set_line("Please wait", 4, BLUE);
            lcd_st7735s_set_line("GPS init..", 5, RED);
            lcd_st7735s_draw_lines();
            DEV_Delay_ms(1000);
        } else {
            lcd_st7735s_set_line("", 4, BLACK);
            lcd_st7735s_set_line("", 5, BLACK);
            lcd_st7735s_draw_lines();
        }
    }
}
