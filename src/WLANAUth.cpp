#include "WLANAuth.h"

bool WLANAuth::validate(std::string token){
    connectWifi();
    httpClient.begin(authURL);
    httpClient.addHeader("Content-Type", "application/json");

    String payload = "{\"token\":\"" + String(token.c_str()) + "\"}";
    int httpResponseCode = httpClient.POST(payload);

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
        httpClient.end();
        return false;
    }
}

void WLANAuth::connectWifi() {
    WiFi.begin(wifiName, wifiPassword);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    Serial.println("Connected to the WiFi network");
}