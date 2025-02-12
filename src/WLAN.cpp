#include "WLAN.h"

WLAN::WLAN(std::string wifiName, std::string wifiPassword) : wifiName(wifiName), wifiPassword(wifiPassword)
{
}

void WLAN::connect()
{
    WiFi.begin(wifiName.c_str(), wifiPassword.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to the WiFi network");
}

void WLAN::disconnect()
{
    WiFi.disconnect();
}

int WLAN::get(std::string url)
{
    httpClient.begin(url.c_str());
    httpClient.addHeader("Content-Type", "application/json");
    return httpClient.GET();
}

int WLAN::post(std::string url, std::string payload)
{
    httpClient.begin(url.c_str());
    httpClient.addHeader("Content-Type", "application/json");
    return httpClient.POST(payload.c_str());
}
