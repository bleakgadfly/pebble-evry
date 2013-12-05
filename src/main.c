#include "pebble.h"
#define LAST_PRICE_SUM_ID 0
#define DIFFERENTIAL_PERCENT_ID 1
#define DIFFERENTIAL_SUM_ID 2

static Window *window; 
static BitmapLayer *image_layer;
//static BitmapLayer *differential_indicator_layer;
//static TextLayer *stock_quote;
//static GBitmap *differential_image;
static GBitmap *image;


void message_handler(DictionaryIterator *iterator, void *context) {
	Tuple* lastPrice_sum = dict_find(iterator, LAST_PRICE_SUM_ID);
	Tuple* differential_in_percent = dict_find(iterator, DIFFERENTIAL_PERCENT_ID);
	Tuple* differential_sum = dict_find(iterator, DIFFERENTIAL_SUM_ID);
}


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
	//app_message_register_inbox_received(message_handler);
}

static void app_deinit(void) {
	//text_layer_destroy(stock_quote);
	//gbitmap_destroy(differential_image);
	gbitmap_destroy(image);
	//bitmap_layer_destroy(differential_indicator_layer);
	bitmap_layer_destroy(image_layer);
	window_destroy(window);
}

int main(void) {
	app_init();
	app_event_loop();
	app_deinit();
}
