#include "lcd_st7735s.h"
#include "wshare_lcd/DEV_Config.h"
#include "wshare_lcd/GUI_Paint.h"
#include "wshare_lcd/LCD_1in8.h"

static UWORD *lcd_buffer = NULL;

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

int lcd_st7735s_draw_string(const char *pString) {
    if (lcd_buffer == NULL) {
        printf("lcd_st7735s_draw_string failed!\n");
        return EXIT_FAILURE;
    }
    Paint_Clear(WHITE);
    Paint_DrawString_EN(1, 1, pString, &Font24, BLACK, WHITE);
    LCD_1IN8_Display(lcd_buffer);
    return EXIT_SUCCESS;
}
