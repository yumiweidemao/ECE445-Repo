#include "weight.h"
#include "esp_log.h"

const static char* TAG = "weight";

// TODO: change weight_value_t to appropriate type
typedef uint16_t weight_value_t;

static weight_value_t last_reading = (weight_value_t)0xFF;
static int64_t start_time;
static int64_t end_time;

static weight_value_t read_weight_value() {
	// TODO: read weight value, preferably a time-averaged value for accuracy
	// TODO: adjust WEIGHT_DIFF_THRESHOLD

	static uint16_t cnt;
	static uint16_t arr[4] = {0x01, 0x03, 0x03, 0x01};
	return (weight_value_t)arr[(cnt++)%4];
}

static void weight_task(void *pvParameters)
{
    while (1) {
    	weight_value_t weight_value = read_weight_value();

    	if (weight_value > last_reading && (weight_value-last_reading > WEIGHT_DIFF_THRESHOLD)) {
    		start_time = esp_timer_get_time();
    	}
    	else if (weight_value < last_reading && (last_reading-weight_value > WEIGHT_DIFF_THRESHOLD)) {
    		end_time = esp_timer_get_time();
    		double time_diff_seconds = (end_time - start_time) / 1000000.0;
    		char msg[20];
    		sprintf(msg, "%.1lf", time_diff_seconds);
    		mqtt_service_publish("ece445/weight", msg);
    	}

    	last_reading = weight_value;

    	vTaskDelay(2500 / portTICK_PERIOD_MS);
    }

    vTaskDelete(NULL);
}

void start_weight_task()
{
    xTaskCreate(weight_task,         // Task function
                "weight_task",       // Task name
                2048,                // Stack size (in words)
                NULL,                // Task input parameter
                4,                   // Task priority
                NULL);               // Task handle (not needed)
    ESP_LOGI(TAG, "weight_task created");
}

