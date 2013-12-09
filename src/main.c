#include "pebble.h"
#define LAST_PRICE_SUM_ID 0
#define DIFFERENTIAL_PERCENT_ID 1
#define DIFFERENTIAL_SUM_ID 2
	
static Window *window; 
static BitmapLayer *logo_bitmap_layer;
static GBitmap *logo_gbitmap_container;
static TextLayer *stock_quote_text_layer;
static TextLayer *time_text_layer;

//static BitmapLayer *arrow_bitmap_layer;
//static GBitmap *arrow_gbitmap_container;


//void message_handler(DictionaryIterator *iterator, void *context) {
//	Tuple* lastPrice_sum = dict_find(iterator, LAST_PRICE_SUM_ID);
//	Tuple* differential_in_percent = dict_find(iterator, DIFFERENTIAL_PERCENT_ID);
//	Tuple* differential_sum = dict_find(iterator, DIFFERENTIAL_SUM_ID);
//}


//void minute_tick_handler(struct tm *tick_time, TimeUnits units_changed) {
//	static char time_text[] = "00:00";
//}

static void text_layer_set_defaults(TextLayer *text_layer) {
	text_layer_set_text_color(text_layer, GColorBlack);
	text_layer_set_background_color(text_layer, GColorWhite);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
}

static Layer* evry_logo_get_layer(GRect *rootBounds) {
	logo_gbitmap_container = gbitmap_create_with_resource(RESOURCE_ID_PEBBLE_EVRY_IMG);
	
	int middleOfScreen = rootBounds->size.h / 2;
	int halfSizeOfLogo = logo_gbitmap_container->bounds.size.h / 2;
	int logoBeginPosition = middleOfScreen - halfSizeOfLogo;
	
	GRect bounds = { 
		.origin = { .x = rootBounds->origin.x, .y = logoBeginPosition	},
		.size = logo_gbitmap_container->bounds.size
	};
	
	logo_bitmap_layer = bitmap_layer_create(bounds);
	bitmap_layer_set_bitmap(logo_bitmap_layer, logo_gbitmap_container);
	bitmap_layer_set_alignment(logo_bitmap_layer, GAlignCenter);
	Layer *logo_layer = bitmap_layer_get_layer(logo_bitmap_layer);
	
	return logo_layer;
}

static Layer* stock_quote_get_layer(GRect *rootBounds) {
	GRect stock_quote_bounds = { 
		.origin = { .x = rootBounds->origin.x, .y = rootBounds->origin.y + 10 }, 
		.size = { .h = 30, .w = rootBounds->size.w }
	};
			
	stock_quote_text_layer = text_layer_create(stock_quote_bounds);
	text_layer_set_defaults(stock_quote_text_layer);
	text_layer_set_text(stock_quote_text_layer, "9,30 | +0,5");
	Layer *stock_quote_layer = text_layer_get_layer(stock_quote_text_layer);
	
	return stock_quote_layer;
}

static Layer* time_layer_get_layer(GRect *rootBounds) {
	GRect time_rect = {
		.origin = { .x = rootBounds->origin.x, .y = rootBounds->size.h - 30 },
		.size = { .h = 30, .w = rootBounds->size.w }
	};
	
	time_text_layer = text_layer_create(time_rect);
	text_layer_set_defaults(time_text_layer);
	text_layer_set_text(time_text_layer, "22:30");
	Layer *time_layer = text_layer_get_layer(time_text_layer);
	
	return time_layer;
}

static void app_init(void) {
	window = window_create();
	window_stack_push(window, false);
	window_set_background_color(window, GColorWhite);
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	Layer *logo_layer = evry_logo_get_layer(&bounds);
	Layer *stock_quote_layer = stock_quote_get_layer(&bounds);
	Layer *time_layer = time_layer_get_layer(&bounds);

	layer_add_child(window_layer, logo_layer);
	layer_add_child(window_layer, time_layer);
	layer_add_child(window_layer, stock_quote_layer);			
	
	//tick_timer_service_subscribe(MINUTE_UNIT, minute_tick_handler);
	//app_message_register_inbox_received(message_handler);
}

static void app_deinit(void) {	
	text_layer_destroy(stock_quote_text_layer);
	text_layer_destroy(time_text_layer);
	//gbitmap_destroy(differential_image);
	gbitmap_destroy(logo_gbitmap_container);
	//bitmap_layer_destroy(differential_indicator_layer);
	bitmap_layer_destroy(logo_bitmap_layer);
	window_destroy(window);
}

int main(void) {
	app_init();
	app_event_loop();
	app_deinit();
}
