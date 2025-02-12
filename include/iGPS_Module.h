#ifndef IGPS_MODULE_H
#define IGPS_MODULE_H

#include "GPSData.h"

class iGPS_Module
{
    public:
        virtual GPSData getGPSData() = 0;
};

#endif // IGPS_MODULE_H