#include <BLECharacteristic.h>
#include <string>

#include "Authentication.h"
#include "WLAN.h"

class TokenCallbacks : public BLECharacteristicCallbacks {
    
    private:
        Internet *internet_module = nullptr;
        std::string auth_url;
    
    public:
        TokenCallbacks(Internet *internet_module, std::string auth_url);
        void onWrite(BLECharacteristic *characteristic) override;
};