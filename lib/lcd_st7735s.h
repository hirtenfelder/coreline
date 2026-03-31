#ifndef SPEEDOMETER_LCD_ST7735S_H
#define SPEEDOMETER_LCD_ST7735S_H
#include "wshare_lcd/GUI_Paint.h"

struct lcd_line_t {
    char text[16];
    UWORD color_foreground;
};

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
 * @brief Acquire the LCD update mutex.
 *
 * Blocks until exclusive access to the ST7735S line buffer and draw
 * operations is available. Call this before performing a group of
 * `lcd_st7735s_set_line()` updates followed by `lcd_st7735s_draw_lines()`
 * so the complete screen update is atomic across both cores.
 *
 * @note Must be paired with `lcd_st7735s_unlock()`.
 * @note This function blocks if another core currently holds the lock.
 */
void lcd_st7735s_lock();

/**
 * @brief Release the LCD update mutex.
 *
 * Releases the mutex previously acquired with `lcd_st7735s_lock()`,
 * allowing other cores to update and render the LCD again.
 *
 * @note Only call this after a matching `lcd_st7735s_lock()`.
 */
void lcd_st7735s_unlock();

/**
 * @brief Sets a line of text in the LCD line buffer.
 *
 * This function copies the provided null-terminated string into the
 * internal line buffer at the specified index. The content will later
 * be rendered by lcd_st7735s_draw_lines().
 *
 * @param text  Null-terminated string to copy into the buffer.
 * @param index Index of the line to update (0 to NUM_LINES - 1).
 * @param color_foreground The foreground text color.
 *
 * @return EXIT_SUCCESS on success.
 * @return EXIT_FAILURE if the index is out of range.
 *
 * @note The destination buffer size must be large enough to hold `line`.
 *       No bounds checking is performed on the string length.
 */
int lcd_st7735s_set_line(const char *text, int index, UWORD color_foreground);

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
