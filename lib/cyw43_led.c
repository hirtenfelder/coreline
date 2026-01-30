#include <stdlib.h>
#include "cyw43_led.h"
#include "pico/cyw43_arch.h"

#define LED_PIN 0
#define LED_ON 1
#define LED_OFF 0

static bool led_init = false;
static bool led_on = false;

int cyw43_led_init() {
    if (cyw43_arch_init()) {
        led_init = false;
        return EXIT_FAILURE;
    }
    led_init = true;
    return EXIT_SUCCESS;
}

int cyw43_led_enable() {
    if (!led_init || led_on) {
        return EXIT_FAILURE;
    }
    cyw43_arch_gpio_put(LED_PIN, LED_ON);
    led_on = true;
    return EXIT_SUCCESS;
}

int cyw43_led_disable() {
    if (!led_init || !led_on) {
        return EXIT_FAILURE;
    }
    cyw43_arch_gpio_put(LED_PIN, LED_OFF);
    led_on = false;
    return EXIT_SUCCESS;
}
