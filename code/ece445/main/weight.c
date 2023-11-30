#include "weight.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "HX711.h"
#include "rake_eventgroup.h"

#define AVG_SAMPLES   10
#define GPIO_DATA   GPIO_NUM_4
#define GPIO_SCLK   GPIO_NUM_5

#define WEIGHT_DIFF_THRESHOLD	20000

static const char* TAG = "weight";

static unsigned long last_reading;
static int64_t start_time, end_time;

static void weight_task(void *pvParameters)
{
	HX711_init(GPIO_DATA,GPIO_SCLK,eGAIN_128);
	HX711_tare();

	unsigned long weight =0;

    while (1) {
    	weight = HX711_get_units(AVG_SAMPLES);
    	ESP_LOGI(TAG, "******* weight = %ld *********\n ", weight);

    	if (weight > last_reading && (weight-last_reading > WEIGHT_DIFF_THRESHOLD)) {
    		start_time = esp_timer_get_time();
    	}
    	else if (weight < last_reading && (last_reading-weight > WEIGHT_DIFF_THRESHOLD)) {
    		end_time = esp_timer_get_time();
    		double time_diff_seconds = (end_time - start_time) / 1000000.0;
    		char msg[20];
    		sprintf(msg, "%.1lf", time_diff_seconds);
    		mqtt_service_publish("ece445/weight", msg);

    		vTaskDelay(6000 / portTICK_PERIOD_MS); // 3s delay before raking after use for safety
    		xEventGroupSetBits(rake_event_group, TOO_STINKY_BIT);
    	}

    	last_reading = weight;

    	vTaskDelay(3000 / portTICK_PERIOD_MS); // measure weight every 3.0 seconds
    }

    vTaskDelete(NULL);
}

void start_weight_task()
{
    xTaskCreate(weight_task,         // Task function
                "weight_task",       // Task name
                2048,                // Stack size (in words)
                NULL,                // Task input parameter
                3,                   // Task priority
                NULL);               // Task handle (not needed)
    ESP_LOGI(TAG, "weight_task created");
}

