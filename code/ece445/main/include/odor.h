#ifndef __ODOR_H__
#define __ODOR_H__

#include "rake_eventgroup.h"
#include "mqtt_service.h"

#define ODOR_MED_THRESHOLD	300
#define ODOR_HI_THRESHOLD	620 // approximately 0.5V

void start_odor_task(void);

#endif /* __ODOR_H__ */
