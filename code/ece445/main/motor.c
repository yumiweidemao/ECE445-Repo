#include "motor.h"
#include "esp_log.h"

const static char* TAG = "motor";

static void rake() {
	// TODO: implement rake function: reads encoder, change GPIO connected to H-Bridge

	ESP_LOGI(TAG, "rake() triggered");
	mqtt_service_publish("ece445/rh", "rake complete");
}

static void motor_task(void *pvParameters)
{
    while (1) {
    	xEventGroupWaitBits(rake_event_group,
			RAKE_COMMAND_BIT | TOO_STINKY_BIT,
			pdFALSE,
			pdFALSE,
			portMAX_DELAY);
		rake();
		xEventGroupClearBits(rake_event_group, RAKE_COMMAND_BIT | TOO_STINKY_BIT);
    }
    vTaskDelete(NULL);
}

void start_motor_task()
{
    xTaskCreate(motor_task,          // Task function
                "motor_task",        // Task name
                2048,                // Stack size (in words)
                NULL,                // Task input parameter
                3,                   // Task priority
                NULL);               // Task handle (not needed)
    ESP_LOGI(TAG, "motor_task created");
}
