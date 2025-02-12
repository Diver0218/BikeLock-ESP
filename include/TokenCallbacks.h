#include <BLECharacteristic.h>
#include <string>

#include "Authentication.h"
#include "WLAN.h"
#include "iLock.h"

class TokenCallbacks : public BLECharacteristicCallbacks {
    
    private:
        Internet *internet_module = nullptr;
        std::string auth_url;
        iLock *lock = nullptr;
    
    public:
        TokenCallbacks(Internet *internet_module, std::string auth_url, iLock *lock);
        void onWrite(BLECharacteristic *characteristic) override;
};