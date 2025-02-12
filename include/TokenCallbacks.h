#include <BLECharacteristic.h>

#include "Authentication.h"
#include "WLAN.h"

class TokenCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) override;
};