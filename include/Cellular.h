

#include "Internet.h"

#include <ArduinoHttpClient.h>
#include <HardwareSerial.h>
#include <string>

class Cellular : public Internet 
{

    private:
        HardwareSerial *modem;
        std::string apn;
        std::string user;
        std::string pass;
        std::string pin;
        int rx;
        int tx;

    public:
        Cellular(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx);
        void connect() override;
        void disconnect() override;
        int get(std::string url) override;
        int post(std::string url, std::string payload) override;
        std::string sendSerial(std::string message, int taskdelay = 1000);
};