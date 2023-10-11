#ifndef __RAKE_EVENTGROUP_H__
#define __RAKE_EVENTGROUP_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define TOO_STINKY_BIT		0x00000001
#define RAKE_COMMAND_BIT	0x00000002

extern EventGroupHandle_t rake_event_group;

void rake_eventgroup_init(void);

#endif /* __RAKE_EVENTGROUP_H__ */
