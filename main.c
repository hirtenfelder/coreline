#include "pico/bootrom.h"
#include "lib/pico_rp2350.h"
#include "lib/lcd_st7735s.h"
#include "wshare_lcd/LCD_1in8.h"
#include "lib/ublox_neo6m.h"

#define DEBUG_ENV_RESET_TIMEOUT (60 * 1000)

int main(void) {
    pico_rp2350_init();

    // add_alarm_in_ms(DEBUG_ENV_RESET_TIMEOUT, pico_rp2350_reset, NULL, false);

    while (true) {
        // tight_loop_contents();
        if (ublox_neo6m_read_next_nmea_sentence()) {
            ublox_neo6m_parse_nmea_sentence();
            const char *timestamp = ublox_neo6m_get_timestamp();
            const char *status = ublox_neo6m_get_status();
            lcd_st7735s_draw_string(timestamp, status);
            DEV_Delay_ms(1000);
            if (strcmp(status, "A") != 0) {
                lcd_st7735s_draw_string("Please wait", "GPS init..");
                DEV_Delay_ms(1000);
            }
        }
    }
}
