#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "SSD1306Wire.h"
#include "secrets.h"

SSD1306Wire display(0x3c, SDA, SCL);

void drawArc(int cx, int cy, int r, int startDeg, int endDeg) {
    for (int deg = startDeg; deg < endDeg; deg++) {
        float a1 = deg * PI / 180.0;
        float a2 = (deg + 1) * PI / 180.0;
        int x1 = cx + r * cos(a1);
        int y1 = cy + r * sin(a1);
        int x2 = cx + r * cos(a2);
        int y2 = cy + r * sin(a2);
        display.drawLine(x1, y1, x2, y2);
    }
}

void drawHappyCat() {
    display.clear();
    display.drawCircle(64, 22, 22);
    display.fillTriangle(46, 4,  52, 16, 42, 16);
    display.fillTriangle(82, 4,  76, 16, 86, 16);
    drawArc(54, 20, 5, 200, 340);
    drawArc(74, 20, 5, 200, 340);
    display.fillTriangle(62, 26, 66, 26, 64, 29);
    drawArc(64, 29, 7, 20, 160);
    display.drawLine(38, 26, 54, 28);
    display.drawLine(38, 31, 54, 30);
    display.drawLine(74, 28, 90, 26);
    display.drawLine(74, 30, 90, 31);
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 48, "opened");
    display.display();
}

void drawSadCat() {
    display.clear();
    display.drawCircle(64, 22, 22);
    display.fillTriangle(46, 6,  50, 16, 40, 18);
    display.fillTriangle(82, 6,  78, 16, 88, 18);
    display.fillCircle(54, 20, 4);
    display.fillCircle(74, 20, 4);
    display.setColor(BLACK);
    display.fillCircle(54, 21, 2);
    display.fillCircle(74, 21, 2);
    display.setColor(WHITE);
    display.fillTriangle(62, 26, 66, 26, 64, 29);
    drawArc(64, 36, 7, 200, 340);
    display.drawLine(38, 28, 54, 29);
    display.drawLine(38, 33, 54, 32);
    display.drawLine(74, 29, 90, 28);
    display.drawLine(74, 32, 90, 33);
    display.fillCircle(78, 27, 2);
    display.fillTriangle(76, 27, 80, 27, 78, 33);
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 48, "closed");
    display.display();
}

void drawConnecting() {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 20, "connecting");
    display.drawString(64, 40, "to wifi...");
    display.display();
}

void drawError() {
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 20, "request");
    display.drawString(64, 40, "error");
    display.display();
}

bool fetchStatus() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi не подключен");
        return false;
    }

    WiFiClient client;
    HTTPClient http;

    http.begin(client, SERVER_URL);
    http.addHeader("accept", "application/json");
    http.addHeader("X-API-KEY", API_KEY);

    int code = http.GET();
    Serial.print("HTTP код: ");
    Serial.println(code);

    if (code != 200) {
        http.end();
        return false;
    }

    String body = http.getString();
    Serial.println(body);
    http.end();

    // Парсим {"status": true/false}
    StaticJsonDocument<64> doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err) {
        Serial.println("JSON ошибка");
        return false;
    }

    return doc["status"].as<bool>();
}

void setup() {
    Serial.begin(115200);

    display.init();
    display.flipScreenVertically();

    drawConnecting();

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Подключение к WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nПодключено! IP: " + WiFi.localIP().toString());
}

void loop() {
    bool isOpen = fetchStatus();

    if (isOpen) {
        drawHappyCat();
    } else {
        drawSadCat();
    }

    delay(1000);  // опрашиваем каждые 5 секунд
}