#include "lcd_st7735s.h"
#include <string.h>

#include "pico/mutex.h"
#include "wshare_lcd/DEV_Config.h"
#include "wshare_lcd/GUI_Paint.h"
#include "wshare_lcd/LCD_1in8.h"

static UWORD *lcd_buffer = NULL;

#define MAX_LEN 16
#define NUM_LINES 6

static mutex_t lcd_mutex;
static struct lcd_line_t lines[NUM_LINES] = {};

int lcd_st7735s_init() {
    DEV_Module_Init();
    LCD_1IN8_Init(HORIZONTAL);
    LCD_1IN8_Clear(WHITE);

    if (lcd_buffer == NULL) {
        UDOUBLE image_size = LCD_1IN8_HEIGHT * LCD_1IN8_WIDTH * 2;
        lcd_buffer = malloc(image_size);
        if (!lcd_buffer) {
            printf("lcd_st7735s_init() failed!\n");
            return EXIT_FAILURE;
        }
        Paint_NewImage((UBYTE *) lcd_buffer, LCD_1IN8.WIDTH, LCD_1IN8.HEIGHT, 0, WHITE);
        Paint_SetScale(65);
        Paint_SetRotate(ROTATE_0);
    }

    for (int i = 0; i < NUM_LINES; i++) {
        lines[i] = (struct lcd_line_t){.text = "", .color_foreground = BLACK};
    }

    mutex_init(&lcd_mutex);

    return EXIT_SUCCESS;
}

void lcd_st7735s_lock() {
    mutex_enter_blocking(&lcd_mutex);
}

void lcd_st7735s_unlock() {
    mutex_exit(&lcd_mutex);
}

int lcd_st7735s_set_line(const char *text, const int index, UWORD const color_foreground) {
    if (index < 0 || index >= NUM_LINES) {
        return EXIT_FAILURE;
    }
    // snprintf prevents overflows when you write longer strings to the display
    snprintf(lines[index].text, sizeof(lines[index].text), "%s", text);
    lines[index].color_foreground = color_foreground;
    return EXIT_SUCCESS;
}

int lcd_st7735s_draw_lines() {
    if (lcd_buffer == NULL) {
        printf("lcd_st7735s_draw_string failed!\n");
        return EXIT_FAILURE;
    }
    Paint_Clear(WHITE);
    int y_start = 8;
    for (int i = 0; i < NUM_LINES; i++) {
        // printf("line %s at %d\n", lines[i].text, y_start);
        Paint_DrawString_EN(4, y_start, lines[i].text, &Font16, lines[i].color_foreground, WHITE);
        y_start += (int) Font16.Height + 4;
    }
    LCD_1IN8_Display(lcd_buffer);
    return EXIT_SUCCESS;
}
