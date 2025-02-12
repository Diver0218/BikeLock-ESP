#include <WiFi.h>
#include <HTTPClient.h>

#include "Internet.h"

class WLAN : public Internet {
    
    private:
        HTTPClient httpClient;
        std::string wifiName;
        std::string wifiPassword;

    public:
        WLAN(std::string wifiName, std::string wifiPassword);
        void connect() override;
        void disconnect() override;
        int get(std::string url) override;
        int post(std::string url, std::string payload) override;
};
