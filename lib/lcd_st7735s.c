#include "lcd_st7735s.h"
#include <string.h>
#include "wshare_lcd/DEV_Config.h"
#include "wshare_lcd/GUI_Paint.h"
#include "wshare_lcd/LCD_1in8.h"

static UWORD *lcd_buffer = NULL;

#define MAX_LEN 16
#define NUM_LINES 6

static char lines[NUM_LINES][MAX_LEN] = {"", "", "", "", "", ""};

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
    return EXIT_SUCCESS;
}

int lcd_st7735s_set_line(const char *line, const int index) {
    if (index < 0 || index >= NUM_LINES) {
        return EXIT_FAILURE;
    }
    strcpy(lines[index], line);
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
        printf("line %s at %d\n", lines[i], y_start);
        Paint_DrawString_EN(4, y_start, lines[i], &Font16, BLACK, WHITE);
        y_start += (int) Font16.Height + 4;
    }
    LCD_1IN8_Display(lcd_buffer);
    return EXIT_SUCCESS;
}
