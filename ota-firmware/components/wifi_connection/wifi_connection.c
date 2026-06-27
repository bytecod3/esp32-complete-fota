/**
* @brief Implement wifi connection functions
* @author Edwin M.
*/

#include "wifi_connection.h"
#include "esp_wifi_types_generic.h"
#include "sec.h" 
#include <string.h>

const char *TAG = "WIFI_SYSTEM";

/**
* @brief handle connection to WIFI
*/
static void handle_wifi_connection(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
	if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {	/* handle WIFI connection bit  */
		esp_wifi_connect();		
		
	} else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
		if(retry_count++ < MAX_RETRY) {
			esp_wifi_connect(); 
			
			wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t *) event_data;
			ESP_LOGI(TAG, "Disconnected, reason=%d", event->reason);
			ESP_LOGI(TAG, "WIFI connect retry: %d", retry_count);
			
		} else {
			xEventGroupSetBits(wifi_events, WIFI_FAILED_BIT);
		}
	} else if(event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {	/* handle IP address assignment */
		ip_event_got_ip_t *event = (ip_event_got_ip_t*) event_data;
		ESP_LOGI(TAG, "ip: %d.%d.%d.%d", IP2STR(&event->ip_info.ip));
		retry_count = 0;
		xEventGroupSetBits(wifi_events, WIFI_CONNECTED_BIT);
	}
}


/**
* @brief initialize wifi subsystem 
* NVS partition is used by the WIFI subsystem
*/
uint8_t init_wifi(void) {
	if(nvs_flash_init() != ESP_OK) {
		nvs_flash_erase();
		nvs_flash_init();
	}
	
	wifi_events = xEventGroupCreate();			/* event group to manage wifi connection*/
	esp_event_loop_create_default();			/* start event loop */
	esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &handle_wifi_connection, NULL);	/* register our own event handler function */
	esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &handle_wifi_connection, NULL); 

	wifi_config_t wifi_config = {  /* wifi configuration parameters */
		.sta = {
			.threshold.authmode = WIFI_AUTH_WPA_PSK,
			.pmf_cfg = {
				.capable = true,
				.required = false,
			}
			
		},
	};
	
	/* update wifi credentialsz, since they are secret variables, I copy them here */
	strcpy((char *) wifi_config.sta.ssid, wifi_ssid);
	strcpy((char *) wifi_config.sta.password, wifi_psd);
	
	esp_netif_init(); 			/* initialise IP address assigner */
	esp_netif_create_default_wifi_sta();
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init(&cfg);
	esp_wifi_set_mode(WIFI_MODE_STA); 		/* this ESP connects to a wifi AP */
	esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
	esp_wifi_start();	
	
	/* block this task until a wifi event occurs */
	EventBits_t bits = xEventGroupWaitBits(
		wifi_events, 
		WIFI_CONNECTED_BIT | WIFI_FAILED_BIT, 
		pdFALSE,
		pdFALSE, 
		portMAX_DELAY
	);
	
	if(bits & WIFI_CONNECTED_BIT) {
		ESP_LOGI(TAG, "WIFI connected");
		return WIFI_CONNECTED_OK;
	} else {
		ESP_LOGI(TAG, "WIFI failed to connect");
		return WIFI_CONNECTION_FAILED;
	}
		
}



















