#include "Authentication.h"
#include <HTTPClient.h>
#include <WiFi.h>

class WLANAuth : public Authentication {
    
    private:
        HTTPClient httpClient;
        char* wifiName = "Das gelobte LAN";
        char* wifiPassword = "joxmag-qAxrad-zimwi2";
    
    public:
        WLANAuth();

        bool validate(std::string token);
        String getCSRFToken();
};