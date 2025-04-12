#include "Cellular_lib.h"
#include "globals.h"


Cellular_lib::Cellular_lib(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx) : pin(pin), apn(apn), user(user), pass(password), rx(rx), tx(tx)
{
}

void Cellular_lib::connect()
{
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);

    serialAT = new HardwareSerial(2);
    serialAT->begin(9600, SERIAL_8N1, 16, 17);

    modem = new TinyGsmClient(*serialAT);
    client = new TinyGsmClient(*modem);

    TinyGsmAutoBaud(*serialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);

    modem.restart();

    if (modem.getSimStatus() != 3)
    {
        modem.simUnlock(pin.c_str());
    }

    modem.gprsConnect(apn.c_str(), user.c_str(), pass.c_str());
    if (modem.isGprsConnected())
    {
        SerialMon.println("GPRS connected");
    }
}

void Cellular_lib::disconnect()
{
    modem.gprsDisconnect();
    modem->radioOff();
    delete modem;
    delete client;
    delete serialAT;
    xSemaphoreGive(gsm_semaphore);
}

int Cellular_lib::get(std::string url)
{
    size_t pos = url.find('/');
    std::string server = url.substr(0, pos);
    std::string resource = url.substr(pos);

    http = new HttpClient(*client, server.c_str(), 80);
    http->get(resource.c_str());
    int statusCode = http->responseStatusCode();
    SerialMon.println("HTTP GET Response: " + std::to_string(statusCode));
    http->stop();
    return statusCode;
}

int Cellular_lib::post(std::string url, std::string payload)
{
    size_t pos = url.find('/');
    std::string server = url.substr(0, pos);
    std::string resource = url.substr(pos);

    http = new HttpClient(*client, server.c_str(), 80);
    http->post(resource.c_str(), "application/json", payload.c_str());
    int statusCode = http->responseStatusCode();
    SerialMon.println("HTTP POST Response: " + std::to_string(statusCode));
    http->stop();
    return statusCode;
}
