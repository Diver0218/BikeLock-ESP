#ifndef GPSDATA_H
#define GPSDATA_H
#include <ctime>

struct GPSData
{
    float latitude;
    float longitude;
    std::tm timestamp;
    GPSData(float latitude, float longitude, std::tm timestamp) : latitude(latitude), longitude(longitude), timestamp(timestamp) {}
};

#endif // GPSDATA_H
