#include <string.h>
#include "esp_log.h"
#include "mqtt_service.h"
#include "rake_eventgroup.h"

static const char *TAG = "mqtt-service";

static esp_mqtt_client_handle_t client;

static uint8_t initialized = 0;

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32, base, event_id);

    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");

        // subscribe to rake topic on successful connection
        ESP_LOGI(TAG, "subscribing to ece445/rake");
        esp_mqtt_client_subscribe(client, "ece445/rake", 0);

        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED");
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "raking command received");
        xEventGroupSetBits(rake_event_group, RAKE_COMMAND_BIT);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", (int)event_id);
        break;
    }
}

void mqtt_service_init() {
	if (initialized) return;
	initialized = 1;
	const esp_mqtt_client_config_t mqtt_cfg = {
		.broker.address.uri = CONFIG_BROKER_URL,
		.session.protocol_ver = MQTT_PROTOCOL_V_5,
	};
	client = esp_mqtt_client_init(&mqtt_cfg);
	esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
	esp_mqtt_client_start(client);
}

int mqtt_service_publish(const char *topic, const char *data) {
	// must be called AFTER mqtt_service_init().
	return esp_mqtt_client_publish(client, topic, data, strlen(data), 0, 0);
}

