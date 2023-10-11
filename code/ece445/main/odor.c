#include "odor.h"
#include "esp_log.h"

const static char* TAG = "odor";

// TODO: change odor_value_t to appropriate type
typedef uint16_t odor_value_t;

static odor_value_t read_odor_value() {
	// TODO: read odor value & adjust odor threshold

	return (odor_value_t)0x04;
}

static void odor_task(void *pvParameters)
{
    while (1) {
    	odor_value_t odor_value = read_odor_value();

    	if (odor_value > (odor_value_t)ODOR_HI_THRESHOLD) {
    		xEventGroupSetBits(rake_event_group, TOO_STINKY_BIT);
    		mqtt_service_publish("ece445/odor", "High");
    	} else if (odor_value > (odor_value_t)ODOR_MED_THRESHOLD) {
    		mqtt_service_publish("ece445/odor", "Medium");
    	} else {
    		mqtt_service_publish("ece445/odor", "Low");
    	}

    	vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void start_odor_task()
{
    xTaskCreate(odor_task,           // Task function
                "odor_task",         // Task name
                2048,                // Stack size (in words)
                NULL,                // Task input parameter
                4,                   // Task priority
                NULL);               // Task handle (not needed)
    ESP_LOGI(TAG, "odor_task created");
}

