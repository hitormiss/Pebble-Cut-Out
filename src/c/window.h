#pragma once
#include <pebble.h>

typedef enum {
	Draw_Text_Fit_Mode_Stretch	= 1,
	Draw_Text_Fit_Mode_Fit 			= 2,
} Draw_Text_Fit_Mode;

//void fctx_set_text_size_stretch(FContext* fctx, FFont* font, int16_t pixels, int scale_from, int scale_to);
//void fctx_draw_string_in_rect(FContext* fctx, const char* text, FFont* font, GRect rect, Draw_Text_Fit_Mode mode);

//static void update_background_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id);
//void set_image(uint8_t image_number);

void load_custom_image();

void main_window_push();
void main_window_update(struct tm *time_now);
