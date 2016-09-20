#pragma once
#include <pebble.h>
#include <ctype.h>
#include "locale.h"
    
typedef struct 
{
	uint8_t	IMAGE;	
	bool FORMAT_TIME;
	bool FORMAT_DATE;
	bool FORMAT_EXTRA;
	bool TIME_24;
	Locale LOCALE;
	GColor COLOR_BACKGROUND;
	GColor COLOR_1;
	GColor COLOR_2;
//} __attribute__((__packed__)) ConfigObject;
} ConfigObject;

extern ConfigObject c;

void initDefaults();

void inbox_received_callback(DictionaryIterator *iterator, void *context);
void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);

