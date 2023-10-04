/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __MQTT_SERVICE_H__
#define __MQTT_SERVICE_H__

#define TAG "MQTT_SERVICE"

#include "esp_system.h"
#include "mqtt_client.h"

/* Function declarations */
void mqtt_service_init(void);
int mqtt_service_publish(const char *topic, const char *data, int len, int qos, int retain);

#endif /* __MQTT_SERVICE_H__ */
