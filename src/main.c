#include "pebble.h"
	
static Window *window; 
static GBitmap *logo_gbitmap_container;
static GBitmap *up_arrow_gbitmap_container;
static GBitmap *down_arrow_gbitmap_container;

static BitmapLayer *logo_bitmap_layer;
static BitmapLayer *arrow_bitmap_layer;
static TextLayer *stock_quote_text_layer;
static TextLayer *time_text_layer;

const int INCREASED_VALUE = 1, STOCK_QUOTE = 2;

void message_handler(DictionaryIterator *iterator, void *context) 
{
	Tuple* has_increased_value = dict_find(iterator, INCREASED_VALUE);
	Tuple* current_stock_quote = dict_find(iterator, STOCK_QUOTE);
	
	if(has_increased_value) {
		switch(has_increased_value->value->int8)
		{
			case 1: // Has increased since previously
				bitmap_layer_set_bitmap(arrow_bitmap_layer, up_arrow_gbitmap_container);
				break;
			case 2: // Has decreased in value
				bitmap_layer_set_bitmap(arrow_bitmap_layer, down_arrow_gbitmap_container);
				break;
			default: 
				break;
		}
	}
	if(current_stock_quote) {
		text_layer_set_text(stock_quote_text_layer, current_stock_quote->value->cstring);
	}
}

void handle_minute_tick(struct tm* tick_time, TimeUnits units_changed) 
{
	static char time_text[] = "00:00";
 	strftime(time_text, sizeof(time_text), "%T", tick_time);
	text_layer_set_text(time_text_layer, time_text);
}

void text_layer_set_defaults(TextLayer *text_layer) 
{
	text_layer_set_text_color(text_layer, GColorBlack);
	text_layer_set_background_color(text_layer, GColorWhite);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
}

Layer* arrow_layer_get_layer(const GRect *rootBounds) 
{
	GRect arrowBounds = {
		.size = up_arrow_gbitmap_container->bounds.size,
		.origin = { .x = 0, .y = 10 }
	};
	
	arrow_bitmap_layer = bitmap_layer_create(arrowBounds);	
	return bitmap_layer_get_layer(arrow_bitmap_layer);
}

Layer* evry_logo_layer_get_layer(const GRect *rootBounds) 
{
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
	return bitmap_layer_get_layer(logo_bitmap_layer);
}

Layer* stock_quote_layer_get_layer(const GRect *rootBounds) 
{
	GRect stock_quote_bounds = { 
		.origin = { .x = 0, .y = 10 }, 
		.size = { .h = 30, .w = rootBounds->size.w }
	};	
	
	stock_quote_text_layer = text_layer_create(stock_quote_bounds);
	text_layer_set_defaults(stock_quote_text_layer);		
	return text_layer_get_layer(stock_quote_text_layer);
}

Layer* time_layer_get_layer(const GRect *rootBounds) 
{
	GRect time_rect = {
		.origin = { .x = rootBounds->origin.x, .y = rootBounds->size.h - 50 },
		.size = { .h = 50, .w = rootBounds->size.w }
	};
	
	time_text_layer = text_layer_create(time_rect);
	text_layer_set_defaults(time_text_layer);
	text_layer_set_font(time_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));	
	return text_layer_get_layer(time_text_layer);
}

static void app_init(void) 
{
	window = window_create();
	window_stack_push(window, false);
	window_set_background_color(window, GColorWhite);
	
	logo_gbitmap_container = gbitmap_create_with_resource(RESOURCE_ID_PEBBLE_EVRY_IMG);
	up_arrow_gbitmap_container = gbitmap_create_with_resource(RESOURCE_ID_UP_ARROW);
	down_arrow_gbitmap_container = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ARROW);

	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	Layer *arrow_layer = arrow_layer_get_layer(&bounds);
	Layer *stock_quote_layer = stock_quote_layer_get_layer(&bounds);
	Layer *logo_layer = evry_logo_layer_get_layer(&bounds);	
	Layer *time_layer = time_layer_get_layer(&bounds);
	
	layer_add_child(window_layer, logo_layer);
	layer_add_child(window_layer, time_layer);
	layer_add_child(window_layer, stock_quote_layer);
	layer_add_child(window_layer, arrow_layer);
		
	time_t now = time(NULL);
	struct tm *current_time = localtime(&now);
	handle_minute_tick(current_time, MINUTE_UNIT);
	tick_timer_service_subscribe(MINUTE_UNIT, &handle_minute_tick);
	app_message_register_inbox_received(message_handler);
	app_message_open(64, 64);
}

static void app_deinit(void) 
{		
	bitmap_layer_destroy(arrow_bitmap_layer);
	bitmap_layer_destroy(logo_bitmap_layer);	

	text_layer_destroy(stock_quote_text_layer);
	text_layer_destroy(time_text_layer);
	
	gbitmap_destroy(up_arrow_gbitmap_container);
	gbitmap_destroy(down_arrow_gbitmap_container);
	gbitmap_destroy(logo_gbitmap_container);
	
	window_destroy(window);
}

int main(void) 
{
	app_init();
	app_event_loop();
	app_deinit();
}
