#include "GPS_Module.h"

GPS_Module::GPS_Module(const char* pin, int rx, int tx)
{
    
    SoftwareSerial SerialAT(rx, tx);
    modem = new TinyGsm(SerialAT);
    modem->begin(pin);
}

GPSData GPS_Module::getGPSData()
{
    float gsm_latitude  = 0;
    float gsm_longitude = 0;
    int   gsm_year      = 0;
    int   gsm_month     = 0;
    int   gsm_day       = 0;
    int   gsm_hour      = 0;
    int   gsm_minute    = 0;
    int   gsm_second    = 0;

    if (modem->getGsmLocation(&gsm_latitude, &gsm_longitude, NULL, &gsm_year, &gsm_month, &gsm_day, &gsm_hour, &gsm_minute, &gsm_second))
    {
        std::tm timestamp;
        timestamp.tm_year = gsm_year - 1900; // tm_year is years since 1900
        timestamp.tm_mon = gsm_month - 1;    // tm_mon is 0-11
        timestamp.tm_mday = gsm_day;
        timestamp.tm_hour = gsm_hour;
        timestamp.tm_min = gsm_minute;
        timestamp.tm_sec = gsm_second;
        return GPSData(gsm_latitude, gsm_longitude, timestamp);    
    }
}