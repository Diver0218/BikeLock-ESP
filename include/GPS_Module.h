#define TINY_GSM_MODEM_SIM800   
#define TINY_GSM_RX_BUFFER   1024

#include <Wire.h>
#include <TinyGsmClient.h>

#include "iGPS_Module.h"


class GPS_Module: public TinyGsmClient, public iGPS_Module
{
    public:
        GPS_Module();

        GPSData getGPSData() override;
};