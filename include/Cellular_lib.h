#define TINY_GSM_MODEM_SIM800
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 115200
#define SerialAT Serial1

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <HardwareSerial.h>
#include <string>

#include "Internet.h"

class Cellular_lib : public Internet
{
    
    private:
        TinyGsm *modem;
        TinyGsmClient *client;
        HttpClient *http;
        std::string apn;
        std::string user;
        std::string pass;
        std::string pin;
        int rx;
        int tx;

    public:
        Cellular_lib(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx);
        void connect() override;
        void disconnect() override;
        int get(std::string url) override;
        int post(std::string url, std::string payload) override;
        std::string sendSerial(std::string message, int taskdelay = 1000);
};