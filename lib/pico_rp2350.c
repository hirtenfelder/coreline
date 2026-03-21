#include "cyw43_led.h"
#include "lcd_st7735s.h"
#include "ublox_neo6m.h"
#include "pico/bootrom.h"
#include "pico/stdio.h"
#include "wshare_lcd/GUI_Paint.h"
#include "wshare_lcd/LCD_1in8.h"
#include "bk_dht22.h"

void pico_rp2350_init() {
    stdio_init_all();

    // Init LED
    cyw43_led_init();
    cyw43_led_enable();

    // Init device and LCD
    lcd_st7735s_init();

    // Init GPS Ublox Neo-6m
    ublox_neo6m_init();

    // Init DHT22 temperature sensor
    dht22_init();
}

int64_t pico_rp2350_reset() {
    LCD_1IN8_Clear(WHITE);
    printf("Entering BOOTSEL mode.\n");
    reset_usb_boot(0, 0);
}
