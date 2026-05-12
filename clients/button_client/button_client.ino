#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "secrets.h"

#define BUTTON_PIN 4 // D2
#define BTN_DEB 50

bool is_open = false;

void sendRequest() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi не подключен");
        return;
    }

    WiFiClient client;
    HTTPClient http;

    http.begin(client, SERVER_URL);
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);

    int code = http.POST("");
    Serial.print("HTTP код: ");
    Serial.println(code);

    if (code > 0) {
        Serial.println(http.getString());
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);

    // Подключение к WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Подключение к WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nПодключено! IP: " + WiFi.localIP().toString());
}

void loop() {
    static uint32_t tmr = 0;
    static bool pState = false;
    static bool stableState = false;

    bool current_state = !digitalRead(BUTTON_PIN);

    if (millis() - tmr >= BTN_DEB) {
        tmr = millis();

        if (current_state != stableState) {
            stableState = current_state;

            if (stableState && !pState) {  
                is_open = !is_open;
                Serial.print("is_open: ");
                Serial.println(is_open);
                sendRequest();  
            }
            pState = stableState;
        }
    }

    digitalWrite(LED_BUILTIN, is_open);
}