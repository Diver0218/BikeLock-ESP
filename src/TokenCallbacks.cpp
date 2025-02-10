#include "TokenCallbacks.h"

void TokenCallbacks::onWrite(BLECharacteristic *characteristic) {
    std::string token = characteristic->getValue();
    Authentication *auth = new WLANAuth();
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