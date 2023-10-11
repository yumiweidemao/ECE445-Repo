#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

EventGroupHandle_t rake_event_group;

void rake_eventgroup_init() {
	rake_event_group = xEventGroupCreate();
}
