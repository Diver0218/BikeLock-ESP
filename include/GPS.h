#include <string>
#include <HardwareSerial.h>

#include "GPSData.h"
#include "iGPS_Module.h"
#include "Internet.h"

class GPS
{

    private:
        std::string url;
        iGPS_Module *module;
        Internet *internet_module = nullptr;

    public:
        GPS(std::string url, iGPS_Module *module, Internet *internet_module);

        GPSData readGPS();
        void uploadGPS(GPSData data);

};