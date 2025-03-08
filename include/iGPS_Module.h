#ifndef IGPS_MODULE_H
#define IGPS_MODULE_H

#include "GPSData.h"

class iGPS_Module
{
    public:
        virtual GPSData getGPSData() = 0;
        virtual void connect() = 0;
        virtual void disconnect() = 0;
};

#endif // IGPS_MODULE_H