#include "pico/bootrom.h"
#include "lib/pico_rp2350.h"
#include "lib/lcd_st7735s.h"
#include "wshare_lcd/LCD_1in8.h"

#define DEBUG_ENV
#define DEBUG_ENV_RESET_TIMEOUT (60 * 1000)

int main(void) {
    pico_rp2350_init();

#ifdef DEBUG_ENV
    add_alarm_in_ms(DEBUG_ENV_RESET_TIMEOUT, pico_rp2350_reset, NULL, false);
#endif

    while (true) {
        // tight_loop_contents();
        lcd_st7735s_draw_string("rp2350");
        DEV_Delay_ms(1000);
        lcd_st7735s_draw_string("st7735s");
        DEV_Delay_ms(1000);
    }
}
