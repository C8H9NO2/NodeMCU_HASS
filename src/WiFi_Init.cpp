//
// Created by cch on 2020/5/14.
//

#include "WiFi_Init.h"

ESP8266WebServer uploadWeb(80);
WiFiClient wifi_client;
PubSubClient client(wifi_client);
IPAddress server{192, 168, 1, 14};

void WiFi_Init::Init_WiFi(const char *ssid, const char *password) {
    Serial.print("Connect to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(50);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Connect Successful");
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
    Setup_MQTT();
}

void WiFi_Init::Init_Updater() {
    ServerResponse();
    uploadWeb.begin();
    Serial.println("Strat Listen");
}

const char *serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
                          "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
                          "<input type='file' name='update'>"
                          "<input type='submit' value='Update'>"
                          "</form>"
                          "<div id='prg'>progress: 0%</div>"
                          "<script>"
                          "$('form').submit(function(e){"
                          "e.preventDefault();"
                          "var form = $('#upload_form')[0];"
                          "var data = new FormData(form);"
                          " $.ajax({"
                          "url: '/update',"
                          "type: 'POST',"
                          "data: data,"
                          "contentType: false,"
                          "processData:false,"
                          "xhr: function() {"
                          "var xhr = new window.XMLHttpRequest();"
                          "xhr.upload.addEventListener('progress', function(evt) {"
                          "if (evt.lengthComputable) {"
                          "var per = evt.loaded / evt.total;"
                          "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                          "}"
                          "}, false);"
                          "return xhr;"
                          "},"
                          "success:function(d, s) {"
                          "console.log('success!')"
                          "},"
                          "error: function (a, b, c) {"
                          "}"
                          "});"
                          "});"
                          "</script>";

void WiFi_Init::ServerResponse() {
    uploadWeb.on("/", HTTP_GET, []() {
        uploadWeb.sendHeader("Connection", "close");
        uploadWeb.send(200, "text/html", serverIndex);
    });


    /*handling uploading firmware file */
    uploadWeb.on("/update", HTTP_POST, []() {
        uploadWeb.sendHeader("Connection", "close");
        uploadWeb.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
        ESP.restart();
    }, []() {

        HTTPUpload &upload = uploadWeb.upload();

        if (upload.status == UPLOAD_FILE_START) {
            Serial.printf("Update: %s\n", upload.filename.c_str());
            if (!Update.begin(UPDATE_ERROR_SIZE)) { //start with max available size
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            /* flashing firmware to ESP*/
            if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
                Update.printError(Serial);
            }
        } else if (upload.status == UPLOAD_FILE_END) {
            if (Update.end(true)) { //true to set the size to the current progress
                Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
            } else {
                Update.printError(Serial);
            }
        }
    });
}

void WiFi_Init::UpdaterHandle() {
    uploadWeb.handleClient();
    delay(1);
    client.loop();
}

void WiFi_Init::Setup_MQTT() {
    client.setServer(server, 1883);
    client.connect("ESP_bedroom", "user", "passwd");
    client.subscribe("ir_server/ac/cmnd/power");
    client.subscribe("ir_server/ac/cmnd/mode");
    client.subscribe("ir_server/ac/cmnd/temp");
    client.subscribe("ir_server/ac/cmnd/fanspeed");
}

void reconnect() {
    client.connect("ESP_bedroom", "user", "passwd");
}

void WiFi_Init::MQTT_Send(const char *topic, const char *payload) {
    if (client.connected()) {
        client.publish(topic, payload);
        Serial.println(payload);
        delay(100);
    } else {
        reconnect();
        MQTT_Send(topic, payload);
    }
}


void WiFi_Init::RegisterCallBack(std::function<void(char *, uint8_t *, unsigned int)> callback) {
    client.setCallback(callback);
}




