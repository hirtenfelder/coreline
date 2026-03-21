#ifndef SPEEDOMETER_BK_DHT22_H
#define SPEEDOMETER_BK_DHT22_H
#include <stdbool.h>

typedef struct {
    // temperature in °C
    float temperature;
    // humidity in %
    float humidity;
} dht22_data_t;

/**
 * @brief Initializes the GPIO pin used for the DHT22 sensor.
 *
 * Configures the DHT22 data pin for use. This must be called before
 * attempting to read data from the sensor.
 *
 * @note The pin is configured but not explicitly set to input/output mode here.
 */
void dht22_init();

/**
 * @brief Reads temperature and humidity data from a DHT22 sensor.
 *
 * This function performs the full DHT22 communication sequence:
 * - Sends start signal to the sensor
 * - Waits for sensor response
 * - Reads 40 bits of data (humidity, temperature, checksum)
 * - Validates checksum
 * - Converts raw values into human-readable floats
 *
 * @param dht22_data Pointer to a dht22_data_t structure where results will be stored.
 *
 * @return true if the read was successful and checksum is valid.
 * @return false if a timeout occurs or checksum validation fails.
 *
 * @note Timing is critical for correct operation; interrupts should not disrupt this function.
 * @note Uses GPIO bit-banging on DHT_PIN.
 * @note Sensor protocol is timing-sensitive (~microsecond resolution).
 */
bool dht22_read(dht22_data_t *dht22_data);

#endif //SPEEDOMETER_BK_DHT22_H
