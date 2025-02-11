#ifndef GPSDATA_H
#define GPSDATA_H

#include <ctime>

struct GPSData
{
    int latitude;
    int longitude;
    int altitude;
    std::time_t timestamp;
    GPSData(int latitude, int longitude, int altitude, std::time_t timestamp) : latitude(latitude), longitude(longitude), altitude(altitude), timestamp(timestamp) {}
};

#endif // GPSDATA_H
