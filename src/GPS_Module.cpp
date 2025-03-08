#include "GPS_Module.h"
#include "globals.h"

GPS_Module::GPS_Module(const char* pin, int rx, int tx)
{
    HardwareSerial *modem = new HardwareSerial(2);
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
    
    Serial.println("Attempting to take semaphore");
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);
    Serial.println("Semaphore taken");

    Serial.println("Failed to get GPS location");
    xSemaphoreGive(gsm_semaphore);
    Serial.println("Semaphore given");
    return GPSData(0, 0, std::tm());
}