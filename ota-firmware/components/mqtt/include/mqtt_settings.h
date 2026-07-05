/*
 * mqtt_settings.h
 *
 *  Created on: Jun 23, 2026
 *      Author: edwin
 */

#ifndef COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_
#define COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_


static const char* COMMANDS_TOPIC = "ota_broker/commands";
static const char* DATA_TOPIC = "ota_broker/data";
static const char* MQTT_CLIENT_ID = "ESP32_OTA_01";

#define MQTT_QOS 			(1)			/* at least once */
#define MQTT_RETAIN 		(1)			/* retain messages */


#define MQTT_VERSION		(4)
#define MQTT_CLEAN			(1)


#endif /* COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_ */
