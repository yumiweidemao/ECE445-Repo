#include "weight.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define HX711_DOUT		GPIO_NUM_4		// GPIO4
#define HX711_SCK		GPIO_NUM_5  	// GPIO5

const static char* TAG = "weight";

typedef int32_t weight_value_t;

static weight_value_t last_reading = (weight_value_t)0x7FFFFFFF;
static int64_t start_time;
static int64_t end_time;

void hx711_init() {
    gpio_set_direction(HX711_DOUT, GPIO_MODE_INPUT);
    gpio_set_direction(HX711_SCK, GPIO_MODE_OUTPUT);

    gpio_set_level(HX711_SCK, 0);
}

static weight_value_t read_weight_value() {
    weight_value_t weight = 0;
    while (gpio_get_level(HX711_DOUT) != 0) {
    	// if data is not ready yet, wait for 100ms
    	vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    for (int i = 0; i < 24; i++) {
    	// SCK high & low time: Min 0.2us, Typ 1us, so we're fine without explicit delays
        gpio_set_level(HX711_SCK, 1);
        weight = weight << 1;
        if (gpio_get_level(HX711_DOUT)) {
            weight++;
        }
        gpio_set_level(HX711_SCK, 0);
    }

    // add 1 extra pulse for now to set gain to 128.
    // This has highest sensitivity but lowest range.
    // 2 extra pulse: gain=64, 3 extra pulse: gain=32
    gpio_set_level(HX711_SCK, 1);
    gpio_set_level(HX711_SCK, 0);

    if (weight & 0x800000) {
    	weight = !(weight & 0x7FFFFF) + 1;
    }

    return weight;
}

static void weight_task(void *pvParameters)
{
	hx711_init();

    while (1) {
    	weight_value_t weight_value = read_weight_value();

    	ESP_LOGI(TAG, "weight value read: 0x%lx", (uint32_t)(weight_value));

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

    	vTaskDelay(2000 / portTICK_PERIOD_MS); // measure weight every 2.0 seconds
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

