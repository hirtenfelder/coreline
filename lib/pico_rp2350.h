#ifndef SPEEDOMETER_PICO_RP2350_H
#define SPEEDOMETER_PICO_RP2350_H

/**
 * @brief Initializes all required hardware for the device.
 *
 * This function sets up standard I/O for debugging, initializes the onboard
 * LED, and calls the LCD initialization routine to prepare the display
 * and framebuffer for use.
 *
 * It should be called once at program startup before any other device-specific
 * functions are used. Ensures that debugging output, LED control, and
 * LCD drawing are ready for subsequent operations.
 */
void pico_rp2350_init();

/**
 * @brief Resets the Pico and enters USB BOOTSEL mode.
 *
 * This function clears the LCD to a blank (WHITE) screen, prints a message
 * to indicate the reset, and triggers the RP2040 bootloader to enter
 * USB mass-storage mode.
 *
 * Important notes:
 * - This function does not return under normal operation.
 * - Use with caution: it immediately halts program execution and
 *   restarts the microcontroller in bootloader mode.
 *
 * @return int64_t Unused; required by the alarm callback signature.
 */
int64_t pico_rp2350_reset();

#endif //SPEEDOMETER_PICO_RP2350_H
