#ifndef SPEEDOMETER_LCD_ST7735S_H
#define SPEEDOMETER_LCD_ST7735S_H

/**
 * @brief Initializes the ST7735S LCD and its persistent framebuffer.
 *
 * This function initializes the LCD hardware and allocates an off-screen
 * framebuffer used for all subsequent drawing operations. The framebuffer
 * is allocated only once and stored in a static variable to ensure it
 * persists for the lifetime of the program.
 *
 * On microcontrollers, repeatedly allocating and freeing large buffers
 * (such as LCD framebuffers) can quickly fragment the heap and cause
 * allocation failures or unstable behavior. By allocating the framebuffer
 * once during initialization and reusing it for every draw operation,
 * memory usage remains deterministic and reliable.
 *
 * This function must be called exactly once before any drawing functions
 * (e.g. lcd_st7735s_draw_string()) are used.
 *
 * @return EXIT_SUCCESS if the LCD was successfully initialized,
 *         EXIT_FAILURE if the LCD initialization has been failed.
 */
int lcd_st7735s_init(void);

/**
 * @brief Draws a text string on the LCD using the persistent framebuffer.
 *
 * This function renders the given null-terminated string into the
 * previously allocated framebuffer and then transfers the framebuffer
 * contents to the LCD.
 *
 * The framebuffer is owned by the LCD driver and is reused across calls.
 * No dynamic memory allocation is performed here, ensuring predictable
 * execution time and avoiding heap fragmentation during normal operation.
 *
 * lcd_st7735s_init() must be called successfully before this function.
 *
 * @param pString1  Null-terminated string to be drawn on the LCD as line one.
 * @param pString2  Null-terminated string to be drawn on the LCD as line two.
 * @return EXIT_SUCCESS if drawing of the string was successfully,
 *         EXIT_FAILURE if drawing of the string has been failed.
 */
int lcd_st7735s_draw_string(const char *pString1, const char *pString2);

#endif //SPEEDOMETER_LCD_ST7735S_H
