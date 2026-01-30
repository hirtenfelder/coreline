#ifndef SPEEDOMETER_CYW43_LED_H
#define SPEEDOMETER_CYW43_LED_H

/**
 * @brief Initialize the CYW43 Wi-Fi/LED subsystem.
 *
 * This function initializes the CYW43 driver, which is required before
 * controlling the WLAN module LED via cyw43_arch_gpio_put().
 *
 * On success, the LED control subsystem is marked as initialized.
 * On failure, no LED operations are permitted.
 *
 * @return EXIT_SUCCESS if initialization succeeds,
 *         EXIT_FAILURE if CYW43 initialization fails.
 *
 * @note Must be called exactly once before enable() or disable().
 * @note This function does NOT change the LED state.
 */
int cyw43_led_init();

/**
 * @brief Turn the WLAN module LED on.
 *
 * Enables the WLAN LED by issuing a write to the CYW43 virtual GPIO.
 * The function will fail if the LED subsystem is not initialized
 * or if the LED is already enabled.
 *
 * @return EXIT_SUCCESS if the LED was successfully enabled,
 *         EXIT_FAILURE if the LED is already on or not initialized.
 *
 * @note This function does not verify the physical LED state;
 *       the state is tracked in software.
 */
int cyw43_led_enable();

/**
 * @brief Turn the WLAN module LED off.
 *
 * Disables the WLAN LED by issuing a write to the CYW43 virtual GPIO.
 * The function will fail if the LED subsystem is not initialized
 * or if the LED is already disabled.
 *
 * @return EXIT_SUCCESS if the LED was successfully disabled,
 *         EXIT_FAILURE if the LED is already off or not initialized.
 *
 * @note This function does not verify the physical LED state;
 *       the state is tracked in software.
 */
int cyw43_led_disable();


#endif //SPEEDOMETER_CYW43_LED_H
