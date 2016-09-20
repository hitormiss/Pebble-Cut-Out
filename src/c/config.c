#include <pebble.h>
#include "config.h"
#include "window.h"
#include "main.h"
#include "pebble_process_info.h"

ConfigObject c;
State g;

#define SAVE_IMAGE						1
#define SAVE_FORMAT_DATE			10
#define SAVE_FORMAT_TIME			11
#define SAVE_FORMAT_EXTRA			12
#define SAVE_TIME_24					15
#define SAVE_LOCALE						16
#define SAVE_COLOR_BACKGROUND	20
#define SAVE_COLOR_1					22
#define SAVE_COLOR_2					23

// Persistence Helpers
#define persist_read_int_safe(key, value) (persist_exists(key) ? persist_read_int(key) : value);
#define persist_read_bool_safe(key, value) (persist_exists(key) ? persist_read_bool(key) : value);
GColor persist_read_color_safe(const uint32_t key, GColor color) {
	if (persist_exists(key)) {
		int x = persist_read_int(key);
		return GColorFromHEX(x);
	}
	else return color;
}

void initDefaults()
{	
	c.IMAGE 				= persist_read_int_safe(SAVE_IMAGE, 1);
	c.FORMAT_DATE 	= persist_read_bool_safe(SAVE_FORMAT_DATE, 0);
	c.FORMAT_TIME 	= persist_read_bool_safe(SAVE_FORMAT_TIME, 0);
	c.FORMAT_EXTRA 	= persist_read_bool_safe(SAVE_FORMAT_EXTRA, 0);
	c.TIME_24				= persist_read_bool_safe(SAVE_TIME_24, 0);

	if (persist_exists(SAVE_LOCALE)) {
		char locale[] = "  ";	
		persist_read_string(SAVE_LOCALE, locale, sizeof("EN"));
		c.LOCALE = set_locale(locale);
	}
	else c.LOCALE = set_locale("EN");
	
	c.COLOR_BACKGROUND 	= persist_read_color_safe(SAVE_COLOR_BACKGROUND, GColorBlack);
	c.COLOR_1		 				= persist_read_color_safe(SAVE_COLOR_1, COLOR_FALLBACK(GColorYellow, GColorWhite));
	c.COLOR_2		 				= persist_read_color_safe(SAVE_COLOR_2, COLOR_FALLBACK(GColorGreen, GColorLightGray));
}	

// --------------------------------------------------------------------------
// Configuration
// --------------------------------------------------------------------------

#define CHUNK_SIZE 1500

void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
	// IMAGE
	Tuple *message_tuple = dict_find(iterator, MESSAGE_KEY_IMAGE);
	if(message_tuple)	{
		c.IMAGE = atoi(message_tuple->value->cstring);
		persist_write_int(SAVE_IMAGE, c.IMAGE);
	}	
	
	// BACKGROUND COLOR
	message_tuple = dict_find(iterator, MESSAGE_KEY_BACKGROUND);
	if(message_tuple) {
		c.COLOR_BACKGROUND = GColorFromHEX(message_tuple->value->int32);
		persist_write_int(SAVE_COLOR_BACKGROUND, message_tuple->value->int32);
	}

	// COLOR 1 FOR GENERATED BACKGROUNDS
	message_tuple = dict_find(iterator, MESSAGE_KEY_COLOR_1);
	if(message_tuple)	{
		c.COLOR_1 = GColorFromHEX(message_tuple->value->int32);
		persist_write_int(SAVE_COLOR_1, message_tuple->value->int32);
	}

	// COLOR 2 FOR GENERATED BACKGROUNDS
	message_tuple = dict_find(iterator, MESSAGE_KEY_COLOR_2);
	if(message_tuple)	{
		c.COLOR_2 = GColorFromHEX(message_tuple->value->int32);
		persist_write_int(SAVE_COLOR_2, message_tuple->value->int32);
	}

	// FORMAT_DATE
	message_tuple = dict_find(iterator, MESSAGE_KEY_FORMAT_DATE);
	if(message_tuple)	{
			c.FORMAT_DATE = atoi(message_tuple->value->cstring);
			persist_write_bool(SAVE_FORMAT_DATE, c.FORMAT_DATE);
	}
	
	// FORMAT_TIME
	message_tuple = dict_find(iterator, MESSAGE_KEY_FORMAT_TIME);
	if(message_tuple)	{
		c.FORMAT_TIME = message_tuple->value->int32;
			persist_write_bool(SAVE_FORMAT_TIME, c.FORMAT_TIME);
	}

	// FORMAT_EXTRA
	message_tuple = dict_find(iterator, MESSAGE_KEY_FORMAT_EXTRA);
	if(message_tuple)	{
		c.FORMAT_EXTRA = message_tuple->value->int32;
			persist_write_bool(SAVE_FORMAT_EXTRA, c.FORMAT_EXTRA);
	}

	// TIME_24
	message_tuple = dict_find(iterator, MESSAGE_KEY_TIME_24);
	if(message_tuple) {
		c.TIME_24 = message_tuple->value->int32;
			persist_write_bool(SAVE_TIME_24, c.TIME_24);
	}

	// LOCALE
	message_tuple = dict_find(iterator, MESSAGE_KEY_LOCALE);
	if(message_tuple)	{
		c.LOCALE = set_locale(message_tuple->value->cstring);	
		persist_write_string(SAVE_LOCALE, message_tuple->value->cstring);
		layer_mark_dirty(g.layer);
	}

	// DOWNLOAD THE PNG
	// Get the received image chunk
	message_tuple = dict_find(iterator, MESSAGE_KEY_DL_DataLength);
  if(message_tuple) {
		g.downloading = true;
		if(g.data_image) free(g.data_image);
		g.data_size = message_tuple->value->int32;
		g.data_image = (uint8_t*)malloc(g.data_size * sizeof(uint8_t));
  }
  // An image chunk
  message_tuple = dict_find(iterator, MESSAGE_KEY_DL_DataChunk);
  if(message_tuple) {
    uint8_t *chunk_data = message_tuple->value->data;

    Tuple *chunk_size_t = dict_find(iterator, MESSAGE_KEY_DL_ChunkSize);
    int chunk_size = chunk_size_t->value->int32;

    Tuple *index_t = dict_find(iterator, MESSAGE_KEY_DL_Index);
    int index = index_t->value->int32;

    // Save the chunk
    memcpy(&g.data_image[index], chunk_data, chunk_size);
  }
  // Complete?
  Tuple *complete_t = dict_find(iterator, MESSAGE_KEY_DL_Complete);
  if(complete_t) {
		if(g.custom_image) {
			gbitmap_destroy(g.custom_image);
			g.custom_image = NULL;
		}
		#ifdef PBL_COLOR
			g.custom_image = gbitmap_create_from_png_data(g.data_image, g.data_size);
		#else
			g.custom_image = gbitmap_create_from_png_data(g.data_image, g.data_size);
			//g.custom_image = gbitmap_create_with_data(g.data_image);
		#endif
		g.downloading = false;
  }	
	if (!g.downloading) layer_mark_dirty(g.layer);
}

void inbox_dropped_callback(AppMessageResult reason, void *context)
{
  ERROR("Message dropped");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
  ERROR("Outbox send failed");
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
	app_message_outbox_send();
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
  ERROR("Outbox send success!");
}
