#include "motor.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define MOTOR1_FORWARD_PIN		GPIO_NUM_6		// GPIO6
#define MOTOR1_BACKWARD_PIN		GPIO_NUM_7		// GPIO7
//#define ENCODER_PIN_A			GPIO_NUM_0		// GPIO0
//#define ENCODER_PIN_B			GPIO_NUM_1		// GPIO1

#define GPIO_OUTPUT_PIN_SEL  ((1ULL<<MOTOR1_FORWARD_PIN) | (1ULL<<MOTOR1_BACKWARD_PIN))
#define GPIO_INPUT_PIN_SEL  ((1ULL<<ENCODER_PIN_A) | (1ULL<<ENCODER_PIN_B))

#define MOTOR_DELAY_MS			27000

const static char* TAG = "motor";

static void forward() {
	gpio_set_level(MOTOR1_FORWARD_PIN, 1);
	gpio_set_level(MOTOR1_BACKWARD_PIN, 0);
}

static void backward() {
	gpio_set_level(MOTOR1_FORWARD_PIN, 0);
	gpio_set_level(MOTOR1_BACKWARD_PIN, 1);
}

static void stop() {
	gpio_set_level(MOTOR1_FORWARD_PIN, 0);
	gpio_set_level(MOTOR1_BACKWARD_PIN, 0);
}

static void rake() {
	// TODO: implement rake function: reads encoder, change GPIO connected to H-Bridge
	// currently, use a timed movement first
	ESP_LOGI(TAG, "forward(), raking start");
	forward();
	vTaskDelay(MOTOR_DELAY_MS / portTICK_PERIOD_MS);
	stop();
	vTaskDelay(1500 / portTICK_PERIOD_MS);
	ESP_LOGI(TAG, "backward()");
	backward();
	vTaskDelay(MOTOR_DELAY_MS / portTICK_PERIOD_MS);
	stop();
	ESP_LOGI(TAG, "stop(), raking complete");

	// post raking complete event to web server
	if (xEventGroupGetBits(rake_event_group) & RAKE_COMMAND_BIT) {
		mqtt_service_publish("ece445/rh", "manual");
	} else { // TOO_STINKY_BIT
		mqtt_service_publish("ece445/rh", "odor");
	}
}

static void motor_task_init() {
	// initialize output pins
	gpio_config_t io_conf = {};
	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
	io_conf.pull_down_en = 1;
	io_conf.pull_up_en = 0;
	gpio_config(&io_conf);

	// TODO: initialize input (encoder) pins. Rising-edge interrupt mode.

	// stop motors from moving
	stop();
}

static void motor_task(void *pvParameters)
{
	motor_task_init();
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
