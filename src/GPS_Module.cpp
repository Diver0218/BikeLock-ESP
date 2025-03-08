#include "GPS_Module.h"


GPS_Module::GPS_Module(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx) : pin(pin), apn(apn), user(user), pass(password), rx(rx), tx(tx)
{
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

    std::string response = sendSerial("AT+CIPGSMLOC=1,1");

    Serial.println("Failed to get GPS location");
    xSemaphoreGive(gsm_semaphore);
    Serial.println("Semaphore given");

    std::istringstream iss(response);
    std::string token;
    std::getline(iss, token, ','); // Skip the first token
    std::getline(iss, token, ',');
    gsm_latitude = std::stof(token);
    std::getline(iss, token, ',');
    gsm_longitude = std::stof(token);
    std::getline(iss, token, ',');
    gsm_year = std::stoi(token.substr(0, 4));
    gsm_month = std::stoi(token.substr(5, 2));
    gsm_day = std::stoi(token.substr(8, 2));
    gsm_hour = std::stoi(token.substr(11, 2));
    gsm_minute = std::stoi(token.substr(14, 2));
    gsm_second = std::stoi(token.substr(17, 2));

    std::tm timeinfo = {};
    timeinfo.tm_year = gsm_year;
    timeinfo.tm_mon = gsm_month;
    timeinfo.tm_mday = gsm_day;
    timeinfo.tm_hour = gsm_hour;
    timeinfo.tm_min = gsm_minute;
    timeinfo.tm_sec = gsm_second;

    return GPSData(gsm_latitude, gsm_longitude, timeinfo);

    return GPSData(0, 0, std::tm());
}

void GPS_Module::connect()
{
    Serial.println("Attempting to take semaphore");
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);
    Serial.println("Semaphore taken");
    modem = new HardwareSerial(2);

    sendSerial("AT+CFUN=1,1", 4000);                        // Modem einschalten
    sendSerial("AT+CPIN=\"" + pin + "\"");                  // PIN setzen
    sendSerial("AT+CGATT=1");                               // GPRS-Anmeldung erzwingen
    sendSerial("AT+SAPBR=0,1");                             // GPRS-Verbindung löschen   
    sendSerial("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");        // GPRS-Verbindungstyp setzen
    sendSerial(("AT+SAPBR=3,1,\"APN\",\"" + apn + "\"").c_str());     // APN konfigurieren
    sendSerial(("AT+SAPBR=3,1,\"USER\",\"" + user + "\"").c_str());   // Benutzername konfigurieren
    sendSerial(("AT+SAPBR=3,1,\"PWD\",\"" + pass + "\"").c_str());    // Passwort konfigurieren
    sendSerial("AT+SAPBR=1,1", 2000);                       // GPRS-Verbindung öffnen
    sendSerial("AT+SAPBR=2,1", 2000);                       // Verbindung prüfen^
}

void GPS_Module::disconnect()
{
    sendSerial("AT+SAPBR=0,1");                             // GPRS-Verbindung löschen
    sendSerial("AT+CGATT=0");                               // GPRS-Anmeldung beenden
    sendSerial("AT+CFUN=0");                            // Modem in den Schlafmodus versetzen
    delete modem;
    xSemaphoreGive(gsm_semaphore);
}

std::string GPS_Module::sendSerial(std::string message, int taskdelay)
{
    Serial.println(("Sending: " + message).c_str());
    modem->println(message.c_str());
    vTaskDelay(taskdelay / portTICK_PERIOD_MS);
    char buffer[2048];
    int index = 0;
    while (modem->available() && index < 2047)
    {
        buffer[index++] = modem->read();
    }
    buffer[index] = '\0';
    Serial.println(("Received: " + std::string(buffer)).c_str());
    return std::string(buffer);
}
