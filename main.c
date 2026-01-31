#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "pico/bootrom.h"
#include "lib/cyw43_led.h"

#define DEV_ENV
#define DEV_ENV_RESET_TIMEOUT (30 * 1000)

int64_t reset_pico() {
    printf("Entering BOOTSEL mode.\n");
    reset_usb_boot(0, 0);
}

int main(void) {
    stdio_init_all();
    cyw43_led_init();
    cyw43_led_enable();

#ifdef DEV_ENV
    add_alarm_in_ms(DEV_ENV_RESET_TIMEOUT, reset_pico, NULL, false);
#endif

    while (true) {
        tight_loop_contents();
    }
}
