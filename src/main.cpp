//
// Created by cch on 2020/5/16.
//

#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <MAX44009.h>
#include <ArduinoJson.h>
#include <ir_Tcl.h>
#include "WiFi_Init.h"

#define IR_LED 4        //because esp8266 in booting time only GPIO 4 and GPIO 5 in low level If use other pin may course boot fail

Adafruit_BME280 bme280;
MAX44009 max44009;
const char *SSID = "";
const char *PassWord = "";
IRTcl112Ac irTcl112Ac(IR_LED);

void callback(char *topic, uint8_t *payload, unsigned int length);

void setup() {
    Serial.begin(74880);
    Wire.begin(0, 5);               //SDA 0,SCL5
    bme280.begin(0x76, &Wire);
    bme280.setSampling(Adafruit_BME280::MODE_NORMAL, Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::SAMPLING_X16,
                       Adafruit_BME280::SAMPLING_X16, Adafruit_BME280::FILTER_X16);
    max44009.begin();
    WiFi_Init::Init_WiFi(SSID, PassWord);
    delay(200);
    WiFi_Init::Init_Updater();
    irTcl112Ac.begin();
    WiFi_Init::RegisterCallBack(callback);
}

int count = 0;

void loop() {
    delay(300);
    if (count > 50) {
        DynamicJsonDocument doc(256);
        doc["Temperature"] = (String) bme280.readTemperature();
        doc["Press"] = (String) (bme280.readPressure() / 100.0F);
        doc["Humidity"] = (String) bme280.readHumidity();
        doc["Lux"] = (String) max44009.get_lux();
        doc["RSSI"] = WiFi.RSSI();
        char temp_char[128];
        serializeJson(doc, temp_char);
        WiFi_Init::MQTT_Send("sensor/bedroom", temp_char);
        count = 0;
    }
    count++;
    WiFi_Init::UpdaterHandle();
}

void callback(char *topic, byte *message, unsigned int length) {
    byte *payload_copy = reinterpret_cast<byte *>(malloc(length + 1));
    if (payload_copy == NULL) {
        Serial.println("Can't allocate memory for `payload_copy`. Skipping callback!");
        return;
    }
    // Copy the payload to the new buffer
    memcpy(payload_copy, message, length);

    // Conversion to a printable string
    payload_copy[length] = '\0';
    String payloadString = String(reinterpret_cast<const char *>(payload_copy));
    String topicString = topic;

    Serial.println("payload:");
    Serial.println(payloadString);
    Serial.println();
    if (topicString == "ir_server/ac/cmnd/power") {
        Serial.println();
        Serial.print(topicString);
        Serial.print(" ");

        if (payloadString == "ON") {
            irTcl112Ac.on();
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "OFF") {
            irTcl112Ac.off();
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
    }
    if (topicString == "ir_server/ac/cmnd/mode") {
        Serial.println();
        Serial.print(topicString);
        Serial.print(" ");
        if (payloadString == "on") {
            irTcl112Ac.on();
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "off") {
            irTcl112Ac.off();
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "cool") {
            irTcl112Ac.setMode(kTcl112AcCool);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "dry") {
            irTcl112Ac.setMode(kTcl112AcDry);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
    }
    if (topicString == "ir_server/ac/cmnd/temp") {
        Serial.println();
        Serial.print(topicString);
        Serial.print(" ");

        irTcl112Ac.setTemp(payloadString.toFloat());
        irTcl112Ac.send();
        delay(200);
        Serial.println(payloadString);
    }
    if (topicString == "ir_server/ac/cmnd/fanspeed") {
        Serial.println();
        Serial.print(topicString);
        Serial.print(" ");

        if (payloadString == "low") {
            irTcl112Ac.setFan(kTcl112AcFanLow);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "medium") {
            irTcl112Ac.setFan(kTcl112AcFanMed);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "high") {
            irTcl112Ac.setFan(kTcl112AcFanHigh);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
        if (payloadString == "auto") {
            irTcl112Ac.setFan(kTcl112AcFanAuto);
            irTcl112Ac.send();
            delay(200);
            Serial.println(payloadString);
        }
    }
}