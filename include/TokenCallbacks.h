#include <BLECharacteristic.h>
#include "WLANAuth.h"

class TokenCallbacks : public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *characteristic) override;
};