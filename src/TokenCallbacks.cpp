#include "TokenCallbacks.h"

TokenCallbacks::TokenCallbacks(Internet *internet_module, std::string auth_url, iLock *lock) : internet_module(internet_module), auth_url(auth_url), lock(lock), BLECharacteristicCallbacks()
{
}

void TokenCallbacks::onWrite(BLECharacteristic *characteristic)
{
    CallbackExecuting = true;
    std::string token = characteristic->getValue();
    Authentication *auth = new Authentication(internet_module, auth_url);
    Serial.print("Token received: ");
    Serial.println(token.c_str());
    Serial.println("Validating token");
    if (auth->validate(token))
    {
        Serial.println("Token is valid");
        lock->toggleLock();
    }
    else
    {
        Serial.println("Token is invalid");
    }
    CallbackExecuting = false;
    bluetoothExecuting = false;
}