#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"
#include "lib/cyw43_led.h"
#include "wshare_lcd/DEV_Config.h"
#include "wshare_lcd/GUI_Paint.h"
#include "wshare_lcd/LCD_1in8.h"

#define DEBUG_ENV
#define DEBUG_ENV_RESET_TIMEOUT (30 * 1000)

int64_t reset_pico() {
    printf("Entering BOOTSEL mode.\n");
    reset_usb_boot(0, 0);
}

int main(void) {
    stdio_init_all();
    cyw43_led_init();
    cyw43_led_enable();

    if (DEV_Module_Init()) {
        return EXIT_FAILURE;
    }

    LCD_1IN8_Init(HORIZONTAL);
    LCD_1IN8_Clear(BLUE);

#ifdef DEBUG_ENV
    add_alarm_in_ms(DEBUG_ENV_RESET_TIMEOUT, reset_pico, NULL, false);
#endif

    while (true) {
        tight_loop_contents();
    }
}
