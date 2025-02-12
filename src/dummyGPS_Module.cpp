#include "dummyGPS_Module.h"

GPSData dummyGPS_Module::getGPSData()
{
    std::tm timestamp = {};
    timestamp.tm_year = 2020 - 1900; // years since 1900
    timestamp.tm_mon = 1 - 1; // months since January
    timestamp.tm_mday = 1;
    timestamp.tm_hour = 6;
    timestamp.tm_min = 6;
    timestamp.tm_sec = 6;
    return GPSData(0, 0, timestamp);
}