#include "GPS_Module.h"

GPS_Module::GPS_Module() : TinyGsmClient()
{

}

GPSData GPS_Module::getGPSData()
{
    // Get GPS data from module
    return GPSData(0, 0, 0, 0);
}