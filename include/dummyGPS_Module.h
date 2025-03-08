#include "iGPS_Module.h"

class dummyGPS_Module : public iGPS_Module
{
    public:
        GPSData getGPSData() override;
        void connect() override;
        void disconnect() override;
};