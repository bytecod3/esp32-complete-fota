/*
 * mqtt_settings.h
 *
 *  Created on: Jun 23, 2026
 *      Author: edwin
 */

#ifndef COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_
#define COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_

static const char* MQTT_HOST = "6602663079714392a702ca7f6f7abb11.s1.eu.hivemq.cloud:8883";
static const char* MQTT_USERNAME = "emwiti";
static const char* MQTT_PASSWORD = "Ene212-0072/2018";

static const char* COMMANDS_TOPIC = "ota_broker/commands";
static const char* DATA_TOPIC = "ota_broker/data";

#define MQTT_QOS 			(1)			/* at least once */
#define MQTT_RETAIN 		(1)			/* retain messages */

static const char* MQTT_CLIENT_ID = "ESP32_OTA_01";
#define MQTT_VERSION	(4)
#define MQTT_CLEAN		(1)



#endif /* COMPONENTS_MQTT_INCLUDE_MQTT_SETTINGS_H_ */
