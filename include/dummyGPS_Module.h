#include "Location.h"

class dummyGPS_Module : public Location
{
    public:
        GPSData getGPSData() override;
        void connect() override;
        void disconnect() override;
};