#include "GPS.h"

GPS::GPS(std::string url, iGPS_Module *module, Internet *internet_module): url(url), module(module), internet_module(internet_module) {}

GPSData GPS::readGPS(){
    module->connect();
    GPSData response =  module->getGPSData();
    module->disconnect();
    return response;
}

void GPS::uploadGPS(GPSData data){
    std::tm tm = data.timestamp;
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
    std::string formatted_time(buffer);
    std::string payload = "{\"latitude\": " + std::to_string(data.latitude) + ", \"longitude\": " + std::to_string(data.longitude) + ", \"timestamp\": \"" + formatted_time + "\"}";
    internet_module->connect();
    internet_module->post(url, payload);
    internet_module->disconnect();
}