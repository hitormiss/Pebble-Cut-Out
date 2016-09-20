#include <pebble.h>
#include <ctype.h>
#include "main.h"
#include "battery_and_health.h"
#include <pebble-fctx/fctx.h>
#include <pebble-fctx/ffont.h>
#include <pebble-fctx/fpath.h>
#include "window.h"
#include "config.h"
#include "locale.h"


// --------------------------------------------------------------------------
// Global state.
// --------------------------------------------------------------------------
State g;


// --------------------------------------------------------------------------
// Tick Handler
// --------------------------------------------------------------------------

static void tick_handler(struct tm *time_now, TimeUnits changed) {
  main_window_update(time_now);
}


// --------------------------------------------------------------------------
// App focus
// --------------------------------------------------------------------------

static void app_focus_changing(bool focusing) {
	if (focusing) {
		Layer *window_layer = window_get_root_layer(g.window);
		layer_set_hidden(window_layer, true); 
	}
}

static void app_focus_changed(bool focused) {
	if (focused) {
		Layer *window_layer = window_get_root_layer(g.window);
		layer_set_hidden(window_layer, false);
		layer_mark_dirty(window_layer);
	}
}

// --------------------------------------------------------------------------
// Unobstructed Area
// --------------------------------------------------------------------------

	static void prv_unobstructed_will_change(GRect final_unobstructed_screen_area, void *context) {
		g.v_bounds = layer_get_bounds(g.layer);
		layer_mark_dirty(g.layer);
	}

	static void prv_unobstructed_did_change(void *context) {
		g.v_bounds = layer_get_unobstructed_bounds(g.layer);
		layer_mark_dirty(g.layer);

		if (grect_equal(&g.bounds, &g.v_bounds)) {
			// do something
		}
	}


// --------------------------------------------------------------------------
// Init
// --------------------------------------------------------------------------

static void init() {
	
	initDefaults();

	main_window_push();
	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
	
	UnobstructedAreaHandlers handlers = {
		.will_change = prv_unobstructed_will_change,
		.did_change = prv_unobstructed_did_change
	};
	unobstructed_area_service_subscribe(handlers, NULL);
	//#endif

	app_focus_service_subscribe_handlers((AppFocusHandlers) {
		.did_focus = app_focus_changed,
		.will_focus = app_focus_changing
	});	
}

static void deinit() { 
	tick_timer_service_unsubscribe();
}

int main() {
  init();
	steps_init();
	load_battery();
  app_event_loop();
	steps_deinit();
	unload_battery();
	deinit();
	return 0;
}
