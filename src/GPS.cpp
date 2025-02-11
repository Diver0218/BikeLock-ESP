#include "GPS.h"

GPS::GPS(std::string url, iGPS_Module *module): url(url), module(module) {}

GPSData GPS::readGPS(){
    return module->getGPSData();
}

void GPS::uploadGPS(GPSData data){
    // Upload data to server
}