#include "bk_dht22.h"

#include "pico/stdlib.h"
#include <stdio.h>

#define DHT_PIN 15

void dht22_init() {
    gpio_init(DHT_PIN);
}

bool dht22_read(dht22_data_t *dht22_data) {
    uint8_t data[5] = {0};

    // Start signal
    gpio_set_dir(DHT_PIN, GPIO_OUT);
    gpio_put(DHT_PIN, 0);
    sleep_ms(2); // >1 ms
    gpio_put(DHT_PIN, 1);
    sleep_us(30);
    gpio_set_dir(DHT_PIN, GPIO_IN);

    // Wait for sensor response
    const absolute_time_t timeout = make_timeout_time_us(100);

    while (gpio_get(DHT_PIN) == 1)
        if (absolute_time_diff_us(get_absolute_time(), timeout) < 0) return false;

    while (gpio_get(DHT_PIN) == 0) {
    }
    while (gpio_get(DHT_PIN) == 1) {
    }

    // Read 40 bits
    for (int i = 0; i < 40; i++) {
        while (gpio_get(DHT_PIN) == 0) {
        }

        const absolute_time_t abs_time = get_absolute_time();

        while (gpio_get(DHT_PIN) == 1) {
        }

        if (absolute_time_diff_us(abs_time, get_absolute_time()) > 40)
            data[i / 8] |= (1 << (7 - (i % 8)));
    }

    // Checksum
    if (((data[0] + data[1] + data[2] + data[3]) & 0xFF) != data[4])
        return false;

    const uint16_t raw_humidity = (data[0] << 8) | data[1];
    const uint16_t raw_temp = (data[2] << 8) | data[3];

    dht22_data->temperature = raw_humidity / 10.0f;
    dht22_data->humidity = (raw_temp & 0x8000) ? -((raw_temp & 0x7FFF) / 10.0f) : (raw_temp / 10.0f);

    return true;
}
