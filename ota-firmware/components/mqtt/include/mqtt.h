/**
* @brief creates MQTT functions
* @author Edwin M.
*/

#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include "mongoose.h"

static struct mg_connection* s_conn;				/* connection handle */					/* main connection handle */
uint8_t mqtt_open;									/* keep track of  if the panel is open and by who */



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
static void mqtt_timer(void* arg);


/**
* @brief publishes data to broker
*/
static void mqtt_data_publish(void* arg);


#endif
