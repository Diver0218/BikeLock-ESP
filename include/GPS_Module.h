#include <Wire.h>
#include <HardwareSerial.h>
#include <ctime>
#include <sstream>

#include "globals.h"
#include "iGPS_Module.h"


class GPS_Module: public iGPS_Module
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
        GPS_Module(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx);

        GPSData getGPSData() override;
        void connect() override;
        void disconnect() override;
        std::string sendSerial(std::string message, int taskdelay = 1000);
};