/**
* Implement MQTT functions
* @author Edwin M.
*/

#include <stdio.h>
#include <string.h>
#include "mqtt.h"
#include "mqtt_settings.h"
#include "mqtt_secrets.h"
#include "mongoose.h"
#include "esp_log.h"

TaskHandle_t mqtt_task_handle = NULL;

const char* MQTT_TAG = "MQTT_SYSTEM";
uint8_t mqtt_open = false;

/**
* @brief Mongoose MQTT event handler
* @param mg_connection pointer to a mongoose connection object
* @param ev event being evaluated
* @param ev_data pointer to data passed to the event loop
*/
static void mqtt_event_handler(struct mg_connection* c, int ev, void* ev_data) {
	if (ev == MG_EV_OPEN) {										  		/* creation of new mg connectino object*/
		MG_INFO(("%lu created", c->id));
		
	} else if(ev == MG_EV_ERROR) {								  		/* on any connection error */
		MG_ERROR(("%lu error %s", c->id, (char*) ev_data));
		
	} else if(ev == MG_EV_TLS_HS) {								   		/* TLS handshake event */
		MG_INFO(("%s\r\n", "TLS handshake complete"));
		
		struct mg_tls_opts tls_opts;
		tls_opts.ca = mg_str();
		tls_opts.name = mg_str(MQTT_HOST);
		
		
		
		
	} else if(ev == MG_EV_MQTT_OPEN) {									/* MQTT connection has been established */
		MG_INFO(("%lu connected to %s\r\n", c->id, MQTT_HOST));
		
		struct mg_str sub_t = mg_str(COMMANDS_TOPIC);				/* subscription and publishing topics */
		struct mg_str pub_t = mg_str(DATA_TOPIC);
		struct mg_str hello_msg = mg_str("Hello from OTA");
		
		struct mg_mqtt_opts sub_opts;									/* subscription options */
		memset(&sub_opts, 0, sizeof(sub_opts));
		sub_opts.topic = sub_t;
		sub_opts.qos = MQTT_QOS;
		
		mg_mqtt_sub(c, &sub_opts);
		
		MG_INFO(("%lu subscribed to %.*s ", c->id, (int) sub_t.len, sub_t.buf));
		
		/* publishing options  */
		struct mg_mqtt_opts pub_opts;
		memset(&pub_opts, 0, sizeof(pub_opts));
		pub_opts.topic = pub_t;
		pub_opts.message = hello_msg;
		pub_opts.qos = MQTT_QOS;
		pub_opts.retain = MQTT_RETAIN;
		
		// todo publish an online message
		
	} else if(ev == MG_EV_MQTT_MSG) {							   /* an MQTT message received from broker */
		
		struct mg_mqtt_message* recvd_payload = (struct mg_mqtt_message*) ev_data;
		
		MG_INFO((										/* debug received message */
			"%lu received %.*s <- %.*s",
			c->id,
			(int) recvd_payload->data.len,
			recvd_payload->data.buf,
			recvd_payload->topic.len,
			recvd_payload->topic.buf
		));
		
		if (mg_match(recvd_payload->topic, mg_str(COMMANDS_TOPIC), NULL)) {
			// todo -> decode message	
		}
				
		
	} else if(ev == MG_EV_CLOSE) {
		MG_INFO(("%lu closed", c->id));
		s_conn = NULL;
		
		mqtt_open = false;
	}
}

/**
* @brief Timer to reconnect to MQTT if connection goes down. 
* This function also initiates the first MQTT connection
* @param arg pointer to any function variable
*/
static void mqtt_reconnect_timer(void* arg) {
	struct mg_mgr* mgr = (struct mg_mgr*) arg;					/** starts all the networking resources */
	
	struct mg_mqtt_opts opts = {
		.client_id = mg_str(MQTT_CLIENT_ID),
		.version = MQTT_VERSION,
		.clean = MQTT_CLEAN
		
	 };
	
	 if (s_conn == NULL) {
		s_conn = mg_mqtt_connect(mgr, MQTT_HOST, &opts, mqtt_event_handler, NULL);
	 }
	
}


/**
* @brief publishes data to broker
*/
static void mqtt_publish_fn(void* arg) {
	ESP_LOGI(MQTT_TAG, "%s", "Dummy data published");
}

/*
* @brief redirect mqtt_log to esp log
*/
static void mg_log_redirect(char ch, void* userdata) {
	ESP_LOGI(MQTT_TAG, "%c", ch);
}

/**
* @brief task that start the mqtt event handler
*/
void mqtt_loop_task(void* args) {
	struct mg_mgr mgr;				/* event manager */
	mg_mgr_init(&mgr);				/* initialise the event manager */
	mg_timer_add(&mgr, MQTT_RECONNECT_PERIOD, MG_TIMER_REPEAT, mqtt_reconnect_timer, &mgr); 	/* reconnection timer */
	mg_timer_add(&mgr, MQTT_DATA_PUBLISH_PERIOD, MG_TIMER_REPEAT, mqtt_publish_fn, &mgr);	/* data publishing timer */
	
	mg_log_set(MG_LL_DEBUG);
	//mg_log_set_fn(mg_log_redirect, NULL);
	
	/* here, MQTT connection is trasnferred to the periodic mqrr_reonnect_timer*/
	MG_INFO(("Starting on %s ", MQTT_HOST));
	
	for(;;) {
		mg_mgr_poll(&mgr, 1000);					/* call event loop every 1 second */
		vTaskDelay(pdMS_TO_TICKS(10));
	}		

}

/**
* @brief spawn mqtt loop task
*/
void init_mqtt() {
	char* mqtt_task_s = "";
	
	if(xTaskCreate(
		mqtt_loop_task,
		"mqtt_loop_task",
		2048,
		NULL,
		1,
		&mqtt_task_handle
	) == pdPASS) {
		mqtt_task_s = "created mqtt_loop_task OK";
	} else {
		mqtt_task_s = "";
	}
	
	ESP_LOGI(MQTT_TAG,"%s", mqtt_task_s);
}































