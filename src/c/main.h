#pragma once
#include <pebble.h>
#include <ctype.h>
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/ffont.h>
#include <pebble-fctx/fpath.h>

#define LOG(...) app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define DEBUG(...) app_log(APP_LOG_LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define INFO(...) app_log(APP_LOG_LEVEL_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define WARN(...) app_log(APP_LOG_LEVEL_WARNING, __FILE__, __LINE__, __VA_ARGS__)
#define ERROR(...) app_log(APP_LOG_LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

// --------------------------------------------------------------------------
// Global state.
// --------------------------------------------------------------------------

typedef struct State {
	Window* window;
	Layer* layer;
  GRect bounds;
  GRect v_bounds;
	GPoint center;
	GPoint v_center;
	
	// CUSTOM IMAGE	
	GBitmap	*custom_image;
	uint8_t	*data_image;
	uint32_t data_size;
	bool downloading;
	
	// TIME & DATE
	uint8_t hours;
	uint8_t minutes;
	uint8_t seconds;
	uint8_t wday;
	uint8_t month;
	uint8_t mday;

	// BATTERY
	uint8_t battery_charge;
	uint8_t battery_charging;
	
	// STEPS
	uint32_t steps;
	
	// FCTX
	FPoint fcenter;
	FPoint v_fcenter;
	FFont* font;	
} State; 

extern State g;
