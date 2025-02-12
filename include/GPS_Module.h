#define TINY_GSM_MODEM_SIM800   
#define TINY_GSM_RX_BUFFER   1024

#include <Wire.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h>
#include <ctime>

#include "iGPS_Module.h"


class GPS_Module: public iGPS_Module
{
    private:
        TinyGsm *modem = nullptr;

    public:
        GPS_Module(const char* pin, int rx, int tx);

        GPSData getGPSData() override;
};