#include "odor.h"
#include "esp_log.h"
#include "driver/adc.h"

#define INTERVAL_MS		3000

const static char* TAG = "odor";

typedef int odor_value_t;

static void init_adc() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_2, ADC_ATTEN_DB_11);
}

static odor_value_t read_odor_value() {
    return (odor_value_t)adc1_get_raw(ADC1_CHANNEL_2);
}

static void odor_task(void *pvParameters)
{
    while (1) {
    	odor_value_t odor_value = read_odor_value();
    	ESP_LOGI(TAG, "odor value:%d", odor_value);

    	if (odor_value > (odor_value_t)ODOR_HI_THRESHOLD) {
    		xEventGroupSetBits(rake_event_group, TOO_STINKY_BIT);
    		mqtt_service_publish("ece445/odor", "High");
    	} else if (odor_value > (odor_value_t)ODOR_MED_THRESHOLD) {
    		mqtt_service_publish("ece445/odor", "Medium");
    	} else {
    		mqtt_service_publish("ece445/odor", "Low");
    	}

    	vTaskDelay(INTERVAL_MS / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void start_odor_task()
{
	init_adc();
    xTaskCreate(odor_task,           // Task function
                "odor_task",         // Task name
                2048,                // Stack size (in words)
                NULL,                // Task input parameter
                4,                   // Task priority
                NULL);               // Task handle (not needed)
    ESP_LOGI(TAG, "odor_task created");
}

