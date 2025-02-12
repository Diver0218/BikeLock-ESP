#include "TokenCallbacks.h"

TokenCallbacks::TokenCallbacks(Internet *internet_module, std::string auth_url) : internet_module(internet_module), auth_url(auth_url), BLECharacteristicCallbacks()
{
}

void TokenCallbacks::onWrite(BLECharacteristic *characteristic)
{
    std::string token = characteristic->getValue();
    Authentication *auth = new Authentication(internet_module, auth_url);
    Serial.print("Token received: ");
    Serial.println(token.c_str());
    if (auth->validate(token))
    {
        Serial.println("Token is valid");
    }
    else
    {
        Serial.println("Token is invalid");
    }
}