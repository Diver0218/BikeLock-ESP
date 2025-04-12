#include <string>
#include <HardwareSerial.h>

#include "GPSData.h"
#include "Location.h"
#include "Internet.h"

class Location_component
{

    private:
        std::string url;
        Location *module;
        Internet *internet_module = nullptr;

    public:
        Location_component(std::string url, Location *module, Internet *internet_module);

        GPSData readGPS();
        void uploadGPS(GPSData data);

};