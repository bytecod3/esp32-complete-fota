/**
* Implement MQTT functions
* @author Edwin M.
*/

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include "esp_system.h"
#include "stdlib.h" // for strtof
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/unistd.h>
#include "mqtt.h"
#include "esp_err.h"
#include "mqtt_settings.h"
#include "mqtt_secrets.h"
#include "mongoose.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "cJSON.h"

TaskHandle_t mqtt_task_handle = NULL;

const char* MQTT_TAG = "MQTT_SYSTEM";
const char* HTTP_TAG = "HTTP_TAG";

uint8_t mqtt_open = false;

/* embedded root CA file todo:use embedtxtfile */
static const char* root_ca = "-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
"-----END CERTIFICATE-----\n";


/*==================OTA variables ===================*/
const char* device_id = "C001";
float firmware_version = 1.0;
float firmware_size_bytes = 0;
char* firmware_filename = "";

float _ota_fw_ver = 0.0;
int _ota_fw_size = 0;
char _ota_fw_bin_name[32];
char _ota_update_mode[32];
char _ota_fw_http_url[128];

static const uint64_t http_conn_timeout = 1500; // HTTP connect timeout in milliseconds
struct mg_mgr http_mgr;

/*========end of OTA variables=====================*/

/**
* @brief OTA completion callback 
*/
static void http_ota_callback(const char *status) {

	if(status == NULL) {
		ESP_LOGI(HTTP_TAG, "OTA success. OTA status: %s", status);
		
		// restart ES
		esp_restart();
	} else {
		ESP_LOGI(HTTP_TAG, "OTA failed. OTA STATUS: %s", status);
	}
	
}

/**
HTTP OTA event handler function
*/ 
static void http_ota_fn(struct mg_connection* c, int ev, void* ev_data) {
	
	if(ev == MG_EV_OPEN) { // connection created, store connect expiration time in c->data
		
		*(uint64_t*) c->data = mg_millis() + http_conn_timeout;
		
	} else if(ev == MG_EV_POLL) {
		if(mg_millis() > *(uint64_t*) c->data && (c->is_connecting || c->is_resolving) ) {
			mg_error(c, "HTTP connection timeout");
		}
		
	} else if(ev == MG_EV_CONNECT) { // connected  to server, send the get request
		// send HTTP get
		ESP_LOGI(HTTP_TAG, "Connected to HTTP server. Sending HTTP GET request");
		struct mg_str _ota_http_url = mg_str((_ota_fw_http_url));
		struct mg_str host = mg_url_host(_ota_fw_http_url);
		const char* uri = mg_url_uri(_ota_fw_http_url);
		
		mg_printf(c, 
			"GET %.*s HTTP/1.1\r\n"
			"Host: %.*s\r\n"
			"Connection: close\r\n"
			"\r\n",
			(int)strlen(uri), uri,
			(int)host.len, host.buf
		);
		
		
	} else if(ev == MG_EV_HTTP_HDRS) {  // http headers received, start OTA streaming
		struct mg_http_message *hm = (struct mg_http_message*) ev_data;
		
		struct mg_str* content_length = mg_http_get_header(hm, "Content-Length");
		ESP_LOGI(HTTP_TAG, "content length: %.*s", (int)hm->uri.len, hm->uri.buf );

	}else if(ev == MG_EV_HTTP_MSG) {
		struct mg_http_message *hm = (struct mg_http_message *) ev_data;
		
	    ESP_LOGI(HTTP_TAG,
	             "HTTP body length: %d",
	             (int) hm->body.len);
	
	    ESP_LOGI(HTTP_TAG,
	             "HTTP body: %.*s",
	             (int) hm->body.len,
	             hm->body.buf);
		
	} else if(ev == MG_EV_ERROR) {				/// todo: close connection on error
		MG_DEBUG(("HTTPS ERROR"));
		
	}
}



/**
* @brief Mongoose MQTT event handler
* @param mg_connection pointer to a mongoose connection object
* @param ev event being evaluated
* @param ev_data pointer to data passed to the event loop
*/
static void mqtt_event_handler(struct mg_connection* c, int ev, void* ev_data) {
	if (ev == MG_EV_OPEN) {										  		/* creation of new mg connectino object*/
		MG_INFO(("%lu created", c->id));
		
	} else if(ev == MG_EV_CONNECT) {
		
		struct mg_tls_opts tls_opts = {0};
		tls_opts.ca = mg_str(root_ca);
		tls_opts.name = mg_str(MQTT_HOST); 							/* without TLS port */
		
		mg_tls_init(c, &tls_opts);
	
	} else if(ev == MG_EV_ERROR) {								  		/* on any connection error */
		MG_ERROR(("%lu error %s", c->id, (char*) ev_data));
		
	} else if(ev == MG_EV_TLS_HS) {								   		/* TLS handshake event */
		MG_INFO(("%s\r\n", "TLS handshake complete. TLS success"));
		
	} else if(ev == MG_EV_MQTT_OPEN) {									/* MQTT connection has been established */
		MG_INFO(("%lu connected to %s\r\n", c->id, MQTT_HOST));
		
		//struct mg_str sub_t = mg_str(COMMANDS_TOPIC);				/* subscription and publishing topics */
		struct mg_str sub_t = mg_str(OTA_TOPIC);					// todo subscribe to multiple topics 
		
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
		
		mg_mqtt_pub(c, &pub_opts);
		
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
		
		if (mg_match(recvd_payload->topic, mg_str(OTA_TOPIC), NULL)) {
			
			/// fetch the firmware meta-data
			cJSON* fw_metadata_obj = cJSON_CreateObject();
			
			if (fw_metadata_obj == NULL) {
				ESP_LOGE(MQTT_TAG, "Could not create CJSON object");
				cJSON_Delete(fw_metadata_obj);
			} else {
				float fw_ota_version = 0.0;
				
				cJSON* fw_metadata = cJSON_Parse(recvd_payload->data.buf);
				
				/////////////////////////////// FETCH FIRMWARE VERSION
				cJSON* ver = cJSON_GetObjectItemCaseSensitive(fw_metadata, "version"); 
				if(!cJSON_IsString(ver)) {
					ESP_LOGE(MQTT_TAG, "Invalid firmware version"); 	// todo: log this to event file
					cJSON_Delete(fw_metadata_obj); 					// free memory
				} else {
					_ota_fw_ver = strtof(cJSON_GetStringValue(ver), NULL);
					ESP_LOGI(MQTT_TAG, "Received firmware version OK: %f\r\n", _ota_fw_ver);
					
					
				}
				
				///////////////////////////////// FETCH FIRMWARE SIZE 
				cJSON* size = cJSON_GetObjectItemCaseSensitive(fw_metadata, "size");			
				if(!cJSON_IsNumber(size)){
					ESP_LOGE(MQTT_TAG, "size cannot be 0");
					cJSON_Delete(fw_metadata_obj);
				} else {
//					errno = 0;
//					char* endptr;
//					
					_ota_fw_size = cJSON_GetNumberValue(size);
					ESP_LOGI(MQTT_TAG, "Received size: %d\r\n", _ota_fw_size);
					
					
					/// check for size conversion errors 
//					if(*endptr != '\0') {
//						ESP_LOGE(MQTT_TAG, "Invalid characters in size ");
//						cJSON_Delete(fw_metadata_obj);
//					} else if(errno != 0) {
//						ESP_LOGE(MQTT_TAG, "Firmware size conversion error");
//						cJSON_Delete(fw_metadata_obj);
//						
//					} else {
//						ESP_LOGI(MQTT_TAG, "Received size: %d\r\n", _ota_fw_size);
//						cJSON_Delete(fw_metadata_obj);						
//					}
				
				}																						
				
				////////////////////////////// FETCH FIRMWARE BIN FILENAME
				cJSON* bin_name = cJSON_GetObjectItemCaseSensitive(fw_metadata, "filename");
				if(!cJSON_IsString(bin_name)) {
					ESP_LOGE(MQTT_TAG, "Invalid OTA bin name");
					cJSON_Delete(fw_metadata_obj);
				} else {
					char* f_name = cJSON_GetStringValue(bin_name);
					size_t fname_len = strlen(cJSON_GetStringValue(bin_name));
					strncpy(_ota_fw_bin_name, f_name, sizeof(_ota_fw_bin_name) - 1);
					_ota_fw_bin_name[sizeof(_ota_fw_bin_name) - 1] = '\0';
					ESP_LOGI(MQTT_TAG, "filename size: %d", fname_len);
					
					ESP_LOGI(MQTT_TAG, "Received filename: %s\r\n", _ota_fw_bin_name);
					
				}
																		
																									
				//////////////////////////////// FETCH UPDATE MODE 
				cJSON* update_mode = cJSON_GetObjectItemCaseSensitive(fw_metadata, "mode");
				if(!cJSON_IsString(update_mode)) {
					ESP_LOGE(MQTT_TAG, "invalid update mode");
					cJSON_Delete(fw_metadata_obj);
				}else {
					char* updt_mode = cJSON_GetStringValue(update_mode);
					size_t mode_len = strlen(updt_mode);
					strncpy(_ota_update_mode, updt_mode, sizeof(_ota_update_mode) -1 );
					_ota_update_mode[sizeof(_ota_update_mode) - 1] = '\0';
					ESP_LOGI(MQTT_TAG, "update mode size: %d", mode_len);
					
					////////////////////////////////////// EXTRACT THE UPDATE URL 
					if(strcmp(updt_mode, "immediate") == 0) {
						ESP_LOGI(MQTT_TAG, "Update the device immediately");
						
						cJSON* url = cJSON_GetObjectItemCaseSensitive(fw_metadata, "url");
						if(!cJSON_IsString(update_mode)) {
							ESP_LOGE(MQTT_TAG, "invalid  update url");
							cJSON_Delete(fw_metadata_obj);
						} else {
							char* updt_url = cJSON_GetStringValue(url);
							size_t url_len = strlen(updt_url);
							strncpy(_ota_fw_http_url, updt_url, sizeof(_ota_fw_http_url) - 1 );
							_ota_fw_http_url[sizeof(_ota_fw_http_url) - 1] = '\0';
							ESP_LOGI(MQTT_TAG, "update url: %s | length: %d", _ota_fw_http_url, url_len);
							
							
							
							// CREATE A NEW HTTP CONNECTION
							// the connection is kept alive by the HTTP_LOOP_TASK
//							mg_mgr_init(&http_mgr);
//							uint8_t done = 0;
//							ESP_LOGI(MQTT_TAG, "Creating new HTTP connection");
//							mg_http_connect(&http_mgr, _ota_fw_http_url, http_ota_fn, &done);		
							
							mg_ota_url_check(&http_mgr, "http://192.168.100.14:8000/updates/metadata" ,  http_ota_callback);
							
						}				
						
					} else if(strcmp(updt_mode, "scheduled") == 0) {
						ESP_LOGI(MQTT_TAG, "Update the device at a scheduled time");
						// todo: implement scheduled updates
					}
					
				}
																								
				//////////////////////////// Free JSON object 
				cJSON_Delete(fw_metadata_obj);
				
			}
			 
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
		.user = mg_str(MQTT_USERNAME),
		.pass = mg_str(MQTT_PASSWORD),
		.version = MQTT_VERSION,
		.clean = MQTT_CLEAN
	 };
	
	 if (s_conn == NULL) {
		s_conn = mg_mqtt_connect(
			mgr, 
			MQTT_HOST_TLS, 
			&opts, 
			mqtt_event_handler, 
			NULL);
	 }
	
}

/**
* @brief publishes data to broker
*/
static void mqtt_publish_fn(void* arg) {
	
	if(s_conn == NULL) {
		ESP_LOGI(MQTT_TAG, "%s", "s_conn is inactive");
	} else {
		struct mg_mqtt_opts pub_opts;
		pub_opts.topic = mg_str_s(DATA_TOPIC);
		pub_opts.message = mg_str("{\"msg\":\"This is OTA\"}");
		pub_opts.qos = 0;
		mg_mqtt_pub(s_conn, &pub_opts);
		
		ESP_LOGI(MQTT_TAG, "%s", "Dummy data published");
		
	}
	
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
	mg_timer_add(
		&mgr, 
		MQTT_RECONNECT_PERIOD, 
		MG_TIMER_REPEAT, 
		mqtt_reconnect_timer, 
		&mgr); 	/* reconnection timer */
		
		
	mg_timer_add(
		&mgr, 
		MQTT_DATA_PUBLISH_PERIOD, 
		MG_TIMER_REPEAT, 
		mqtt_publish_fn, 
		&mgr);	/* data publishing timer */
		
	mg_log_set(MG_LL_DEBUG);
	//mg_log_set_fn(mg_log_redirect, NULL);
	
	/* here, MQTT connection is transferred to the periodic mqrr_reonnect_timer*/
	MG_INFO(("Starting on %s ", MQTT_HOST));
	
	for(;;) {
		mg_mgr_poll(&mgr, 1000);					/* call event loop every 1 second */
		
		vTaskDelay(pdMS_TO_TICKS(10));
	}		

}

/**
* @brief Task to handle Polling of HTTP manager for OTA streaming 
*/
void http_loop_task(void* args) {
	mg_mgr_init(&http_mgr);
	
	for(;;) {
		mg_mgr_poll(&http_mgr, 500); /* call event loop every one 500ms*/
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}


void init_nvs() {
	esp_err_t err = nvs_flash_init();
	if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		err = nvs_flash_init();
	}
	
	ESP_ERROR_CHECK(err);
	
	// open NVS handle 
	ESP_LOGI(MQTT_TAG, "\nOpening Non-volative storage handle");
	nvs_handle_t my_handle;
	
	err = nvs_open("storage", NVS_READWRITE, &my_handle);
	if(err != ESP_OK) {
		ESP_LOGE(MQTT_TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
		return;
	}
	
	// store an read an integer value 
	int32_t counter = 42;
	ESP_LOGI(MQTT_TAG, "\nWriting counter to NVS");
	err = nvs_set_i32(my_handle, "counter", counter);
	if(err != ESP_OK) {
		ESP_LOGE(MQTT_TAG, "Failed to write counter");
	}
	
	// read back the stored value 
	int32_t read_counter = 0;
	ESP_LOGI(MQTT_TAG, "\nReading value from NVS");
	err = nvs_get_i32(my_handle, "counter", &read_counter);
	
	switch (err) {
		case ESP_OK:
			ESP_LOGI(MQTT_TAG, "Read counter = %" PRIu32, read_counter);
			break;
		
		case ESP_ERR_NVS_NOT_FOUND:
			ESP_LOGW(MQTT_TAG, "The value is not initialized yet");
			break;
		
		default:
			ESP_LOGE(MQTT_TAG, "Error (%s) reading", esp_err_to_name(err));
	
	}
	
}


/**
* @brief spawn mqtt loop task
*/
void init_mqtt() {
	char* mqtt_task_s = "";
	
	/* initialise NVS storage */
	
	/* create MQTT loop task */
	if(xTaskCreate(
		mqtt_loop_task,
		"mqtt_loop_task",
		8000,
		NULL,
		1,
		&mqtt_task_handle
	) == pdPASS) {
		mqtt_task_s = "created mqtt_loop_task OK";
	} else {
		mqtt_task_s = "";
	}
	
	/* init HTTP poll */
	if(xTaskCreate(
		http_loop_task,
		"http_loop_task",
		6000,
		NULL,
		1,
		NULL
	) == pdPASS) {
		ESP_LOGI(HTTP_TAG, "Created HTTP loop task");
	} else{ 
		ESP_LOGI(HTTP_TAG, "Failed to create HTTP loop task");
	}
		
	ESP_LOGI(MQTT_TAG,"%s", mqtt_task_s);
}






























