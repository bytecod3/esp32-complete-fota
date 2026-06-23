/**
* Implement MQTT functions
* @author Edwin M.
*/

#include <stdio.h>
#include "mqtt.h"
#include "esp_partition.h"
#include "mongoose.h"

/**
* @brief Mongoose MQTT event handler
* @param mg_connection pointer to a mongoose connection object
* @param ev event being evaluated
* @param ev_data pointer to data passed to the event loop
*/
static void mqtt_event_handler(struct mg_connection* c, int ev, void* ev_data) {
	if (ev == MG_EV_OPEN) {										  /* creation of new mg connectino object*/
		MG_INFO(("%lu created", c->id));
		
	} else if(ev == MG_EV_ERROR) {								  /* on any connection error */
		MG_EV_ERROR(("%lu error %s", c->id, (char*) ev_data));
		
	} else if(ev == MG_EV_TLS_HS) {								   /* TLS handshake event */
		MG_INFO(("%s\r\n", "TLS handshake complete"));
		
	} else if(ev == MG_EV_MQTT_OPEN) {								/* MQTT connection has been established */
		MG_INFO(("%lu connected to %s\r\n", c->id, MQTT_HOST));
		
		/* subscription and publishing topics */
		struct mg_str sub_t = mg_str(COMMANDS_TOPIC);
		struct mg_str pub_t = mg_str(DATA_TOPIC);
		
		struct mg_mqtt_opts sub_opts;
		memset(&sub_opts, 0, sizeof(sub_opts));
		sub_opts.topic = sub_t;
		sub_opts.qos = MQTT_QOS;
		
		
		
		
	} else if(ev == MG_EV_MQTT_MSG) {							   /* an MQTT message received from broker */
		
	}
}

/**
* @brief Timer to reconnect to MQTT if connection goes down. 
* This function also initiates the first MQTT connection
* @param arg pointer to any function variable
*/
static void mqtt_timer(void* arg) {
	
}