#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

#include "mqtt_service.h"
#include "wifi_station.h"
#include "rake_eventgroup.h"
#include "motor.h"
#include "odor.h"
#include "weight.h"

static const char *TAG = "app-main";

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi connection and MQTT client
    start_wifi_init_task();

    // Initialize rake event group
    rake_eventgroup_init();

    // Start other tasks
    start_motor_task();
    start_odor_task();
    start_weight_task();

    while (1) {
    	ESP_LOGI(TAG, "Hello!");
    	sleep(30);
    }
}
