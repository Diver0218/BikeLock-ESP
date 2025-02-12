#include "TokenCallbacks.h"

void TokenCallbacks::onWrite(BLECharacteristic *characteristic) {
    std::string token = characteristic->getValue();
    Authentication *auth = new Authentication(new WLAN("Das gelobte LAN", "joxmag-qAxrad-zimwi2"), "http://192.168.178.49:3498/LockAuth/");
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