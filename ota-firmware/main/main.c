#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "mqtt.h"
#include "wifi_connection.h"

#define IDLE_TASK_DELAY (10)

void app_main(void) {
	
	init_wifi();
	
    while (true) {
		vTaskDelay(pdMS_TO_TICKS(IDLE_TASK_DELAY));
        
    }
}
