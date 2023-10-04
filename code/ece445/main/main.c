#include "mqtt_service.h"
#include "wifi_station.h"

void app_main(void)
{
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize Wi-Fi station-mode connection.
    // Note: Please configure Wi-Fi credentials in sdkconfig
    // TODO: do not crash if Wi-Fi is not connected. Keep running everything locally.
    ESP_ERROR_CHECK(wifi_init_sta());

    // Initialize MQTT client service
    mqtt_service_init();

    while (1) {
    	ESP_LOGI("main_test", "publishing test msg to ece445/test");
    	mqtt_service_publish("ece445/test", "Hello, litter box!", 0, 1, 0);
    	sleep(15);
    }
}
