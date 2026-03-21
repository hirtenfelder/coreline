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
 * @brief Sets a line of text in the LCD line buffer.
 *
 * This function copies the provided null-terminated string into the
 * internal line buffer at the specified index. The content will later
 * be rendered by lcd_st7735s_draw_lines().
 *
 * @param line  Null-terminated string to copy into the buffer.
 * @param index Index of the line to update (0 to NUM_LINES - 1).
 *
 * @return EXIT_SUCCESS on success.
 * @return EXIT_FAILURE if the index is out of range.
 *
 * @note The destination buffer size must be large enough to hold `line`.
 *       No bounds checking is performed on the string length.
 */
int lcd_st7735s_set_line(const char *line, int index);

/**
 * @brief Renders all stored text lines to the ST7735S LCD.
 *
 * This function clears the display buffer, then draws all strings stored
 * in the internal `lines[]` buffer line by line using Font16. Each line
 * is vertically spaced by font height + 4 pixels. After drawing, the
 * buffer is sent to the physical LCD.
 *
 * @return EXIT_SUCCESS on successful rendering.
 * @return EXIT_FAILURE if the LCD buffer is not initialized.
 *
 * @note Requires `lcd_buffer` to be initialized before calling.
 * @note Uses white background and black text for rendering.
 * @note Line spacing is controlled by `Font16.Height + 4`.
 */
int lcd_st7735s_draw_lines(void);

#endif //SPEEDOMETER_LCD_ST7735S_H
