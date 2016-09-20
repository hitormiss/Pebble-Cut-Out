#include "main.h"
#include "window.h"
#include "config.h"
#include "locale.h"
//#include "pebble_process_info.h"

// --------------------------------------------------------------------------
// Global state.
// --------------------------------------------------------------------------
State g;

// --------------------------------------------------------------------------
// CUSTOM IMAGE
// --------------------------------------------------------------------------

void load_custom_image() {
	//LOG("Load_custom_image()");
	g.downloading = true;
	if(g.custom_image) {
		//LOG("no custom image");
		gbitmap_destroy(g.custom_image);
		g.custom_image = NULL;
  }

	DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
	  // Add a key-value pair
	  dict_write_uint8(iter, 0, 0);
	  // Send the message!
  app_message_outbox_send();
}


// --------------------------------------------------------------------------
// Drawing Functions
// --------------------------------------------------------------------------

GRect GRect_Zone(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
	GRect rect = GRect(g.v_bounds.size.w/4*x + g.v_bounds.origin.x, 
										 g.v_bounds.size.h/4*y + g.v_bounds.origin.y, 
										 g.v_bounds.size.w/4*w, 
										 g.v_bounds.size.h/4*h);
	return rect;
}

void fctx_set_text_size_stretch(FContext* fctx, FFont* font, int16_t pixels, int scale_from, int scale_to) {
    fctx->transform_scale_from.x = FIXED_TO_INT(font->units_per_em);
    fctx->transform_scale_from.y = -fctx->transform_scale_from.x;
    fctx->transform_scale_to.x = pixels * scale_to / scale_from;
    fctx->transform_scale_to.y = pixels;
}

void fctx_draw_string_in_rect(FContext* fctx, const char* text, FFont* font, GRect rect, Draw_Text_Fit_Mode mode) {
	
		// set text size to height of rect
		fctx_set_text_size(fctx, font, rect.size.h);
	
		// determine default width if not fit to rect
    uint16_t code_point;
    uint16_t decode_state = 0;
    const char* p;
		fixed_t default_text_width = 0;
		for (p = text; *p; ++p) {
			if (0 == decode_utf8_byte(*p, &decode_state, &code_point)) {
				FGlyph* glyph = ffont_glyph_info(font, *p);
				if (glyph) {
					default_text_width += glyph->horiz_adv_x;
				}
			}
		}
		default_text_width = FIXED_TO_INT(default_text_width * fctx->transform_scale_to.x / fctx->transform_scale_from.x);

		if (mode == Draw_Text_Fit_Mode_Stretch)	{
			fctx_set_text_size_stretch(fctx, font, rect.size.h, default_text_width, rect.size.w);
		}
		else {
			
			if (default_text_width >= rect.size.w) {
				uint8_t new_h = (rect.size.h* rect.size.w / default_text_width);
				fctx_set_text_size(fctx, font, new_h);
			}				
			else {
				// Text Size already set
			}
		}

		// set offset to center of the rect
		fctx_set_offset(fctx, g2fpoint(grect_center_point(&rect)));
		fctx_draw_string(fctx, text, font, GTextAlignmentCenter, FTextAnchorMiddle);	
}	

// --------------------------------------------------------------------------
// Layer Functions
// --------------------------------------------------------------------------

static void layer_update_proc(Layer *layer, GContext *ctx) {

	static FContext fctx;
	GRect rect;
	GRect inset_bounds = grect_inset(g.v_bounds, GEdgeInsets(5));
	Draw_Text_Fit_Mode stretch_mode = Draw_Text_Fit_Mode_Stretch;
	char text[] = "        ";	

	
	// CUSTOM IMAGE
	if (c.IMAGE == 10) {
		if(!g.custom_image && !g.downloading) load_custom_image();
		if(!g.downloading) {
			graphics_draw_bitmap_in_rect(ctx, g.custom_image, PBL_IF_ROUND_ELSE(inset_bounds, g.v_bounds));
		}
	}
	// OTHER BACKGROUND IMAGE
	else if (c.IMAGE < 20) {
		if(g.custom_image) {
			gbitmap_destroy(g.custom_image);
			g.custom_image = NULL;
		}
		GBitmap *image;
		#ifdef PBL_COLOR
			if (c.IMAGE == 4) image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_FLOWERS); 
			else if (c.IMAGE == 2) image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_BEACH); 
			else if (c.IMAGE == 3) image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_GRASS); 
			else image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_OCEAN); 
		#else
			if (c.IMAGE == 3) image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_DIAGONAL); 
			else if (c.IMAGE == 2) image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_BRICK); 
			else image = gbitmap_create_with_resource(RESOURCE_ID_BACKGROUND_OCEAN); 
		#endif

		graphics_draw_bitmap_in_rect(ctx, image, PBL_IF_ROUND_ELSE(inset_bounds, g.v_bounds));
		gbitmap_destroy(image);	
	}
	// No Background Image
	else {
		graphics_context_set_fill_color(ctx, c.COLOR_1);
		graphics_fill_rect(ctx, g.bounds, 0, GCornerNone);

		graphics_context_set_fill_color(ctx, c.COLOR_2);
		// STRIPES: Horizontal
		if (c.IMAGE == 20) {
			for(int y=0; y<=24; y++) {
				rect = GRect(0, g.bounds.size.h/24 * y, g.bounds.size.w, g.bounds.size.h/24);
				if (y%2) graphics_fill_rect(ctx, rect, 0, GCornerNone);			
			}
		}
		// STRIPES: Vertical
		else if (c.IMAGE == 21) {
			for(int x=0; x<=24; x++) {
				rect = GRect(g.bounds.size.w/24 * x, 0, g.bounds.size.w/24, g.bounds.size.h);
				if (x%2) graphics_fill_rect(ctx, rect, 0, GCornerNone);			
			}
		}
		// POLKA DOT
		else if (c.IMAGE == 25) {
			for(int y=0; y<=(inset_bounds.size.h/12); y++) {
				for(int x=0; x<=(inset_bounds.size.w/12); x++) {
					if ((x+y)%2) continue;
					graphics_fill_circle(ctx, GPoint(inset_bounds.origin.x + (12*x)+6, inset_bounds.origin.y + (12*y)+6), 6);
				}
			}		
		}
		// CHECKERED
		else if (c.IMAGE == 26) {
			for(int y=0; y<=(g.bounds.size.h/10); y++) {
				for(int x=0; x<=(g.bounds.size.w/10); x++) {
					if ((x+y)%2) continue;
					rect = GRect(10*x, 10*y, 10, 10);
					graphics_fill_rect(ctx, rect, 0, GCornerNone);			
				}
			}
		}
	}	
	fctx_init_context(&fctx, ctx);
	fctx_begin_fill(&fctx);
	
	if (c.IMAGE) {
		//fctx_set_color_bias(&fctx, -1);
		fctx_set_fill_color(&fctx, c.COLOR_BACKGROUND);

		#if defined(PBL_ROUND)
			fctx_plot_circle(&fctx, &g.fcenter, INT_TO_FIXED(89));
		#else
			fctx_move_to(&fctx, FPointI(0, 0));
			fctx_line_to(&fctx, FPointI(0, g.bounds.size.h));
			fctx_line_to(&fctx, FPointI(g.bounds.size.w, g.bounds.size.h));
			fctx_line_to(&fctx, FPointI(g.bounds.size.w, 0));
			fctx_close_path(&fctx);
		#endif 
	}
	else {
		fctx_set_fill_color(&fctx, c.COLOR_1);
	}

	// DRAW HOURS
	if (c.FORMAT_EXTRA) {
		if (!c.FORMAT_TIME) rect = GRect_Zone(0, 0, 2, 2);
		else rect = GRect_Zone(2, 0, 2, 2);
		rect = grect_inset(rect, GEdgeInsets(5, (c.FORMAT_TIME?5:2), 3, (c.FORMAT_TIME?2:5)));
	}
	else {
		if (c.FORMAT_TIME) {
			rect = GRect_Zone(0, 0, 2, 2);
			rect = grect_inset(rect, GEdgeInsets(5, 2, 3, 5));
		}
		else {
			rect = GRect_Zone(0, 0, 2, 4);
			rect = grect_inset(rect, GEdgeInsets(5, 2, 5, 5));
		}
	}
	uint8_t hours_use = g.hours;
	if (c.TIME_24==0) { 
		if (hours_use > 12) hours_use -= 12; 
		if (hours_use == 0) hours_use = 12; 
	}
	if (c.FORMAT_EXTRA)	snprintf(text, sizeof("000000"), "%02d", hours_use);
	else snprintf(text, sizeof("000000"), "%d", hours_use);
	fctx_draw_string_in_rect(&fctx, text, g.font, rect, stretch_mode);

	// DRAW MINUTES
	if (c.FORMAT_EXTRA) {
		if (!c.FORMAT_TIME) rect = GRect_Zone(0, 2, 2, 2);
		else rect = GRect_Zone(2, 2, 2, 2);
		rect = grect_inset(rect, GEdgeInsets(3, (c.FORMAT_TIME?5:2), 5, (c.FORMAT_TIME?2:5)));
	}
	else {
		if (c.FORMAT_TIME) {
			rect = GRect_Zone(2, 0, 2, 4);
			rect = grect_inset(rect, GEdgeInsets(5, 5, 5, 2));
		}
		else {
			rect = GRect_Zone(2, 0, 2, 2);
			rect = grect_inset(rect, GEdgeInsets(5, 5, 3, 2));
		}
	}
	snprintf(text, sizeof("000000"), "%02d", g.minutes);
	fctx_draw_string_in_rect(&fctx, text, g.font, rect, stretch_mode);

	// DRAW WEEKDAY
	if (!c.FORMAT_TIME) rect = GRect_Zone(2, 2, 2, 1);
	else rect = GRect_Zone(0, 2, 2, 1);
	rect = grect_inset(rect, GEdgeInsets(3, (c.FORMAT_TIME?2:5), 3, (c.FORMAT_TIME?5:2)));
	fctx_draw_string_in_rect(&fctx, get_weekday(g.wday, c.LOCALE), g.font, rect, stretch_mode);

	// DRAW MONTH/DATE
	if (!c.FORMAT_TIME) rect = GRect_Zone(2, 3, 2, 1);
	else rect = GRect_Zone(0, 3, 2, 1);
	rect = grect_inset(rect, GEdgeInsets(3, (c.FORMAT_TIME?2:5), 5, (c.FORMAT_TIME?5:2)));
	if (c.FORMAT_DATE) snprintf(text, sizeof("000000"), "%d/%d", g.mday, g.month+1);
	else snprintf(text, sizeof("000000"), "%d/%d", g.month+1, g.mday);
	fctx_draw_string_in_rect(&fctx, text, g.font, rect, stretch_mode);
	
	if(c.FORMAT_EXTRA) {
		// DRAW BATTERY
		if (!c.FORMAT_TIME) rect = GRect_Zone(2, 0, 2, 1);
		else rect = GRect_Zone(0, 0, 2, 1);
		rect = grect_inset(rect, GEdgeInsets(5, (c.FORMAT_TIME?2:5), 3, (c.FORMAT_TIME?5:2)));
		if (g.battery_charging) snprintf(text, sizeof("000000"), "%d+", g.battery_charge);
		else snprintf(text, sizeof("000000"), "%d%%", g.battery_charge);
		fctx_draw_string_in_rect(&fctx, text, g.font, rect, stretch_mode);

		// DEBUG FOR CLOUD PEBBLE
		//g.steps = 3500;

		// DRAW STEPS
		if (!c.FORMAT_TIME) rect = GRect_Zone(2, 1, 2, 1);
		else rect = GRect_Zone(0, 1, 2, 1);
		rect = grect_inset(rect, GEdgeInsets(3, (c.FORMAT_TIME?2:5), 3, (c.FORMAT_TIME?5:2)));
		if (g.steps <= 999) snprintf(text, sizeof("000"), "%03d", (int)g.steps);
		if (g.steps <= 9999) snprintf(text, sizeof("0000"), "%04d", (int)g.steps);
		else if(g.steps <= 99999) snprintf(text, sizeof("0000"), "%d.%dK", (int)(g.steps/1000), (int)((g.steps%1000)/100));
		else snprintf(text, sizeof("0000"), "%dK", (int)(g.steps/1000));
		fctx_draw_string_in_rect(&fctx, text, g.font, rect, stretch_mode);
	}
	
	fctx_end_fill(&fctx);
	fctx_deinit_context(&fctx);	
}


// --------------------------------------------------------------------------
// Windows Functions
// --------------------------------------------------------------------------

static void window_load(Window *window) {

	g.custom_image = NULL;
	g.data_image = NULL;
	g.downloading = false;
	
  Layer *window_layer = window_get_root_layer(window);
  g.bounds = g.v_bounds = layer_get_bounds(window_layer);
	
	// SET INITIAL TIME
	time_t time_n;
	time(&time_n);
	struct tm *time_now = localtime(&time_n);
  g.hours = time_now->tm_hour;
  g.minutes = time_now->tm_min;
  g.seconds = time_now->tm_sec;
  g.wday = time_now->tm_wday;
  g.mday = time_now->tm_mday;
  g.month = time_now->tm_mon;

	g.layer = layer_create(g.bounds);
	layer_set_update_proc(g.layer, layer_update_proc);
	layer_add_child(window_layer, g.layer);

	g.center = g.v_center = grect_center_point(&g.bounds);
	g.fcenter = g.v_fcenter = g2fpoint(g.center);

	#ifdef PBL_ROUND
  	g.v_bounds = GRect(25, 25, 130, 130);
		g.v_center = grect_center_point(&g.v_bounds);
		g.v_fcenter = g2fpoint(g.center);
	#endif 

	g.font = ffont_create_from_resource(RESOURCE_ID_FONT);
	
	// ENABLE CLAY CONFIG PAGE	
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);
  app_message_open(1600, 30); 
}

static void window_unload(Window *window) {
	
	ffont_destroy(g.font);
	layer_destroy(g.layer);

	if(g.custom_image) gbitmap_destroy(g.custom_image);
  if(g.data_image) free(g.data_image);
	
	window_destroy(g.window);
}

void main_window_push() {
  g.window = window_create();
  window_set_background_color(g.window, GColorWhite);
  window_set_window_handlers(g.window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(g.window, true);
}

void main_window_update(struct tm *time_now) {
  g.hours = time_now->tm_hour;
  g.minutes = time_now->tm_min;
  g.seconds = time_now->tm_sec;
  g.wday = time_now->tm_wday;
  g.mday = time_now->tm_mday;
  g.month = time_now->tm_mon;
	
	layer_mark_dirty(g.layer);
}
