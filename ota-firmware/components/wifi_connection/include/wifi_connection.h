/**
* @brief Implement wifi connection functions
* @author Edwin M.
*/
#ifndef COMPONENTS_MQTT_INCLUDE_WIFI_CONNECTION_H
#define COMPONENTS_MQTT_INCLUDE_WIFI_CONNECTION_H

#include <stdint.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "portmacro.h"

#define WIFI_CONNECTED_BIT 	(BIT0)
#define WIFI_FAILED_BIT 	(BIT1)
#define MAX_RETRY			(10)		/* number of times to try reconnecting WIFI */

extern const char* wifi_ssid;
extern const char* wifi_psd;

static EventGroupHandle_t wifi_events;		/* used to pass wifi events across the system */
static int retry_count = 0;

extern const char *TAG;

static void request_page(void);
static void handle_wifi_connection(void*, esp_event_base_t, int32_t, void*);
void init_wifi(void);

#endif /* COMPONENTS_MQTT_INCLUDE_WIFI_CONNECTION_H */
