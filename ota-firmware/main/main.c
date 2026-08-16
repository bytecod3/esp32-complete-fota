#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "hal/gpio_types.h"
#include "mqtt.h"
#include "portmacro.h"
#include "soc/gpio_struct.h"
#include "wifi_connection.h"
#include "driver/gpio.h"

#define IDLE_TASK_DELAY (10)

/* GPIO definitions */
#define GPIO_LED (2)
#define GPIO_LED_SEL (1ULL << GPIO_LED)
static TickType_t next = 0;
static bool led_state = false;

/* RTOS task prototypes */
void app_main(void) {
	
	init_nvs();		/* initialise NVS storage */
	
	if(init_wifi()) {		/* initialise WIFI subsystem */
		init_mqtt();		/* initialise MQTT subsystem */
	}	
	
	/* set up GPIO */
	gpio_config_t io_conf;
	
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = GPIO_LED_SEL;
	io_conf.pull_down_en = 0;
	io_conf.pull_up_en = 0;
	
	gpio_config(&io_conf);
		
	
	
    while (true) {
		
		/* blink LED */
		TickType_t now = xTaskGetTickCount();
		
		if(now > next) {
			led_state = !led_state;
			gpio_set_level(GPIO_LED, led_state);
			next = now + 200 / portTICK_PERIOD_MS;
			
			ESP_LOGI("MAIN", "Calling main loop led blink ota-ED BITCH");
		}
		
		vTaskDelay(pdMS_TO_TICKS(IDLE_TASK_DELAY));
        
    }
	
}
