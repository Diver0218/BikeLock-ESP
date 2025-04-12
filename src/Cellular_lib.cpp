#include "Cellular_lib.h"
#include "globals.h"


Cellular_lib::Cellular_lib(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx) : pin(pin), apn(apn), user(user), pass(password), rx(rx), tx(tx)
{
}

void Cellular_lib::connect()
{
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);

    modem = new TinyGsm(SerialAT);
    client = new TinyGsmClient(*modem);

    Serial.println("Initializing modem...");
    SerialAT.begin(9600, SERIAL_8N1, rx, tx);

    sendSerial("AT+CFUN=1,1", 4000);                        // Modem einschalten
    sendSerial("AT");                  

    // Serial.println("Modem initializing...");
    // if (!modem->restart()) {
    //     Serial.println("Failed to restart modem");
    //     xSemaphoreGive(gsm_semaphore);
    //     return;
    // }

    Serial.println("Entering modem PIN...");
    if (modem->getSimStatus() != 3)
    {
        modem->simUnlock(pin.c_str());
    }

    Serial.println("Connect GPRS...");
    if (!modem->gprsConnect(apn.c_str(), user.c_str(), pass.c_str())) {
        Serial.println("Failed to connect to GPRS");
        xSemaphoreGive(gsm_semaphore);
        return;
    }
    Serial.println("GPRS connected");
    sendSerial("AT+SAPBR=2,1");                             // GPRS-Verbindung löschen
}

void Cellular_lib::disconnect()
{
    modem->gprsDisconnect();
    modem->radioOff();
    delete modem;
    delete client;
    xSemaphoreGive(gsm_semaphore);
}

int Cellular_lib::get(std::string url)
{
    size_t pos = url.find('/');
    std::string server = url.substr(0, pos);
    std::string resource = url.substr(pos);

    Serial.println(("Server: " + server).c_str());
    Serial.println(("Resource: " + resource).c_str());

    http = new HttpClient(*client, server.c_str(), 80);
    if (!http) {
        Serial.println("Failed to create HTTP client");
        return -1;
    }
    int err = http->get(resource.c_str());
    if (err != 0) {
        Serial.println(("HTTP GET Error: " + std::to_string(err)).c_str());
        http->stop();
        delete http;
        return -1;
    }
    int statusCode = http->responseStatusCode();
    Serial.println(("HTTP GET Response: " + std::to_string(statusCode)).c_str());
    http->stop();
    delete http;
    return statusCode;

}
int Cellular_lib::post(std::string url, std::string payload)
{
    size_t pos = url.find('/');
    std::string server = url.substr(0, pos);
    std::string resource = url.substr(pos);

    Serial.println(("Server: " + server).c_str());
    Serial.println(("Resource: " + resource).c_str());

    http = new HttpClient(*client, server.c_str(), 80);
    if (!http) {
        Serial.println("Failed to create HTTP client");
        return -1;
    }
    int err = http->post(resource.c_str());
    if (err != 0) {
        Serial.println(("HTTP GET Error: " + std::to_string(err)).c_str());
        http->stop();
        delete http;
        return -1;
    }
    int statusCode = http->responseStatusCode();
    Serial.println(("HTTP POST Response: " + std::to_string(statusCode)).c_str());
    http->stop();
    return statusCode;
}

std::string Cellular_lib::sendSerial(std::string message, int taskdelay)
{
    Serial.println(("Sending: " + message).c_str());
    SerialAT.println(message.c_str());
    vTaskDelay(taskdelay / portTICK_PERIOD_MS);
    char buffer[2048];
    int index = 0;
    while (SerialAT.available() && index < 2047)
    {
        buffer[index++] = SerialAT.read();
    }
    buffer[index] = '\0';
    Serial.println(("Received: " + std::string(buffer)).c_str());
    return std::string(buffer);
}
