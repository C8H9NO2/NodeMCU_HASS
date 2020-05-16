//
// Created by cch on 2020/5/14.
//

#ifndef EPS32S_AMP_MPU9250_WIFI_INIT_H
#define EPS32S_AMP_MPU9250_WIFI_INIT_H

#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include "PubSubClient.h"

class WiFi_Init {
private:
    static void ServerResponse();

    static void Setup_MQTT();

//    static void reconnect();

public:
    static void Init_WiFi(const char *ssid, const char *password);

    static void Init_Updater();

    static void MQTT_Send(const char *topic, const char *payload);

    static void UpdaterHandle();

    static void RegisterCallBack(MQTT_CALLBACK_SIGNATURE);
};


#endif //EPS32S_AMP_MPU9250_WIFI_INIT_H
