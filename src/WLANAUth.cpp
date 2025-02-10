#include "WLANAuth.h"
#include <WiFi.h>
WLANAuth::WLANAuth() {
    // Initialize WiFi and HTTPClient
    WiFi.begin();
    HTTPClient httpClient;
}

bool WLANAuth::validate(std::string token){
    WiFi.begin(wifiName, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to the WiFi network");

    if (WiFi.status() == WL_CONNECTED) {
        httpClient.begin(authURL);
        httpClient.addHeader("Content-Type", "application/json");

        String payload = "{\"token\":\"" + String(token.c_str()) + "\"}";
        Serial.print("Payload: ");
        Serial.println(payload);

        int httpResponseCode = httpClient.POST(payload);
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        if (httpResponseCode == 200) {
            httpClient.end();
            return true;
        }
        else if (httpResponseCode == 401) {
            httpClient.end();
            return false;
        }
        else {
            // server / network error
            // handle error
            Serial.println("Error on HTTP request");
            httpClient.end();
            return false;
        }
    } else {
        Serial.println("Failed to connect to WiFi");
        return false;
    }
}