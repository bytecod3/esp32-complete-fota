/**
* @brief creates MQTT functions
* @author Edwin M.
*/

#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include "freertos/idf_additions.h"
#include "mongoose.h"

#define MQTT_RECONNECT_PERIOD		(3000)
#define MQTT_DATA_PUBLISH_PERIOD	(2000)

extern TaskHandle_t mqtt_task_handle;

extern const char* MQTT_TAG;


static struct mg_connection* s_conn;				/* connection handle */					/* main connection handle */
extern uint8_t mqtt_open;									/* keep track of  if the panel is open and by who */

/* task prototypes */
void mqtt_loop_task(void* args);


/**
* @brief find MQTT next topic
*/
static size_t mg_mqtt_next_topic(struct mg_mqtt_message *msg,struct mg_str *topic, uint8_t *qos, size_t pos);
					
/**
* @brief
*/
			 
/**
* @brief Mongoose MQTT event loop
*/
static void mqtt_event_handler(struct mg_connection* c, int ev, void* ev_data);


/**
* @brief Timer to reconnect to MQTT if connection goes down. 
* This function also initiates the first MQTT connection
*/
static void mqtt_reconnect_timer(void* arg);


/**
* @brief publishes data to broker
*/
static void mqtt_publish_fn(void* arg);

/*
* @brief redirect mqtt_log to esp log
*/
static void mg_log_redirect(char ch, void* userdata);

/**
* @brief creates the mqtt loop task that start the mqtt event handler
*/
void init_mqtt();

/**
* initialise NVS
*/
void init_nvs();


#endif
