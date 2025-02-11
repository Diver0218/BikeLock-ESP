#include <string>

#include "GPSData.h"
#include "iGPS_Module.h"

class GPS
{

    private:
        std::string url;
        iGPS_Module *module;

    public:
        GPS(std::string url, iGPS_Module *module);

        GPSData readGPS();
        void uploadGPS(GPSData data);

};