#include "pebble.h"

static Window *window; 
static BitmapLayer *image_layer;
static GBitmap *image;
//static TextLayer *time_text_layer;

void minute_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
	static char time_text[] = "00:00";
	static char date_text[] = "";
}

static void app_init(void) {
	window = window_create();
	window_stack_push(window, false);
	
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	
	image = gbitmap_create_with_resource(RESOURCE_ID_PEBBLE_EVRY_IMG);
	image_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_bitmap(image_layer, image);
	bitmap_layer_set_alignment(image_layer, GAlignCenter);	
	layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));
	
	//tick_timer_service_subscribe(MINUTE_UNIT, minute_tick_handler);
}

static void app_deinit(void) {
	gbitmap_destroy(image);
	bitmap_layer_destroy(image_layer);
	window_destroy(window);
}

int main(void) {
    app_init();
    app_event_loop();
    app_deinit();
}
