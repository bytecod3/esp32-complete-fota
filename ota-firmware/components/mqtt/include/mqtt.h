/**
* @brief creates MQTT functions
* @author Edwin M.
*/

#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include "mongoose.h"
#include "mqtt_settings.h"


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
