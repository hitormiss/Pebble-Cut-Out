#include <pebble.h>
#include "main.h"
#include "battery_and_health.h"
#include <pebble-simple-health/pebble-simple-health.h>
#include <pebble-events/pebble-events.h>

// --------------------------------------------------------------------------
// Global state.
// --------------------------------------------------------------------------
State g;

// --------------------------------------------------------------------------
// Battery Functions
// --------------------------------------------------------------------------

static EventHandle battery_handle;

// Check current battery level, change colors and set percentage level.
static void battery_bar_battery_update(BatteryChargeState charge_state) {

	if ((g.battery_charge == charge_state.charge_percent) &&
			(g.battery_charging == charge_state.is_charging)) {
		//APP_LOG(APP_LOG_LEVEL_DEBUG_VERBOSE, "battery charge percent doesnt change, skip");
		return;
	}	
	g.battery_charge = charge_state.charge_percent;
	g.battery_charging = charge_state.is_charging;
	//layer_mark_dirty(g.layer);
}

void load_battery() {
	battery_bar_battery_update(battery_state_service_peek());
	battery_handle = events_battery_state_service_subscribe(&battery_bar_battery_update);
}

void unload_battery() {
	events_battery_state_service_unsubscribe(battery_handle);
}

// --------------------------------------------------------------------------
// Health / Steps Functions
// --------------------------------------------------------------------------

void update_steps_count() {
  g.steps = health_get_metric_sum(HealthMetricStepCount);
}

void steps_init() {
  if (!health_is_available()) {
    DEBUG("Health service is not available");
  }
	else {
	  health_init(update_steps_count);
	}
}
 
void steps_deinit(){
  health_deinit();
}