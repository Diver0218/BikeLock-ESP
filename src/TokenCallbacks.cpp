#include "TokenCallbacks.h"

void TokenCallbacks::onWrite(BLECharacteristic *characteristic) {
    std::string token = characteristic->getValue();
    Authentication *auth = new WLANAuth();
}