#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "mqtt.h"
#include "wifi_connection.h"

#define IDLE_TASK_DELAY (10)

/* rtos task prototypes */


void app_main(void) {
	
	if(init_wifi()) {		/* initialise WIFI subsystem */
		init_mqtt();		/* initialise MQTT subsystem */
	}	
	
    while (true) {
		vTaskDelay(pdMS_TO_TICKS(IDLE_TASK_DELAY));
        
    }
}
