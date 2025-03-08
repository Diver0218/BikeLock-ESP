#include <Wire.h>
#include <HardwareSerial.h>
#include <ctime>

#include "iGPS_Module.h"


class GPS_Module: public iGPS_Module
{
    private:
        HardwareSerial *modem;

    public:
        GPS_Module(const char* pin, int rx, int tx);

        GPSData getGPSData() override;
};