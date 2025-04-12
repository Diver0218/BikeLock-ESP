#include "Cellular_location.h"


Cellular_location::Cellular_location(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx) : pin(pin), apn(apn), user(user), pass(password), rx(rx), tx(tx)
{
}

GPSData Cellular_location::getGPSData()
{
    float gsm_latitude  = 0;
    float gsm_longitude = 0;
    int   gsm_year      = 0;
    int   gsm_month     = 0;
    int   gsm_day       = 0;
    int   gsm_hour      = 0;
    int   gsm_minute    = 0;
    int   gsm_second    = 0;

    std::string response = sendSerial("AT+CIPGSMLOC=1,1", 3000);

    int newline = response.find('\n');
    Serial.println("1. Check");
    int comma1 = response.find(',', newline + 1);
    Serial.println("2. Check");
    int comma2 = response.find(',', comma1 + 1);
    Serial.println("3. Check");
    int comma3 = response.find(',', comma2 + 1);
    Serial.println("4. Check");
    int slash1 = response.find('/', comma3 + 1);
    int slash2 = response.find('/', slash1 + 1);
    int comma4 = response.find(',', slash2 + 1);
    int colon1 = response.find(':', slash2 + 1);
    int colon2 = response.find(':', colon1 + 1);

    //+CIPGSMLOC: 0,0.000000,0.000000,2025/03/08,15:28:41
    Serial.println("5. Check");
    std::string s_gsm_latitude = "";
    Serial.println("6. Check");
    for(int i = comma1 + 1; i < comma2; i++)
    {
        s_gsm_latitude += response[i];
    }
    Serial.println(("7. Check" + s_gsm_latitude).c_str());
    gsm_latitude = std::stof(s_gsm_latitude);
    
    Serial.println(("8. Check" + std::to_string(gsm_latitude)).c_str());
    std::string s_gsm_longtitude = "";
    Serial.println("9. Check");
    for(int i = comma2 + 1; i < comma3; i++)
    {
        s_gsm_longtitude += response[i];
    }
    Serial.println(("10. Check" + s_gsm_longtitude).c_str());
    gsm_longitude = std::stof(s_gsm_longtitude);
    Serial.println(("11. Check" + std::to_string(gsm_longitude)).c_str());

    std::string s_gsm_year;
    for(int i = comma3 + 1; i < slash1; i++)
    {
        s_gsm_year = s_gsm_year + response[i];
    }
    gsm_year = std::stoi(s_gsm_year) - 1900;

    std::string s_gsm_month;
    for(int i = slash1 + 1; i < slash2; i++)
    {
        s_gsm_month = s_gsm_month + response[i];
    }
    gsm_month = std::stoi(s_gsm_month) - 1;

    std::string s_gsm_day;
    for(int i = slash2 + 1; i < comma4; i++)
    {
        s_gsm_day = s_gsm_day + response[i];
    }
    gsm_day = std::stoi(s_gsm_day);

    std::string s_gsm_hour;
    for(int i = comma4 + 1; i < colon1; i++)
    {
        s_gsm_hour = s_gsm_hour + response[i];
    }
    gsm_hour = std::stoi(s_gsm_hour) + 1;       //Timezone correction

    std::string s_gsm_minute;
    for(int i = colon1 + 1; i < colon2; i++)
    {
        s_gsm_minute = s_gsm_minute + response[i];
    }
    gsm_minute = std::stoi(s_gsm_minute);

    std::string s_gsm_second;
    for(int i = colon2 + 1; i < response.length(); i++)
    {
        s_gsm_second = s_gsm_second + response[i];
    }



    // std::istringstream iss(response);
    // std::string token;
    // std::getline(iss, token, ','); // Skip the first token
    // std::getline(iss, token, ',');
    // gsm_latitude = std::stof(token);
    // std::getline(iss, token, ',');
    // gsm_longitude = std::stof(token);
    // std::getline(iss, token, ',');
    // gsm_year = std::stoi(token.substr(0, 4));
    // gsm_month = std::stoi(token.substr(5, 2));
    // gsm_day = std::stoi(token.substr(8, 2));
    // gsm_hour = std::stoi(token.substr(11, 2));
    // gsm_minute = std::stoi(token.substr(14, 2));
    // gsm_second = std::stoi(token.substr(17, 2));

    std::tm timeinfo = {};
    timeinfo.tm_year = gsm_year;
    timeinfo.tm_mon = gsm_month;
    timeinfo.tm_mday = gsm_day;
    timeinfo.tm_hour = gsm_hour;
    timeinfo.tm_min = gsm_minute;
    timeinfo.tm_sec = gsm_second;

    return GPSData(gsm_latitude, gsm_longitude, timeinfo);
}

void Cellular_location::connect()
{
    Serial.println("Attempting to take semaphore");
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);
    Serial.println("Semaphore taken");
    serialAT = new HardwareSerial(2);
    serialAT->begin(9600, SERIAL_8N1, rx, tx);

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

void Cellular_location::disconnect()
{
    sendSerial("AT+SAPBR=0,1");                             // GPRS-Verbindung löschen
    sendSerial("AT+CGATT=0");                               // GPRS-Anmeldung beenden
    sendSerial("AT+CFUN=0", 4000);                            // Modem in den Schlafmodus versetzen
    delete serialAT;
    xSemaphoreGive(gsm_semaphore);
}

std::string Cellular_location::sendSerial(std::string message, int taskdelay)
{
    Serial.println(("Sending: " + message).c_str());
    serialAT->println(message.c_str());
    vTaskDelay(taskdelay / portTICK_PERIOD_MS);
    char buffer[512];
    int index = 0;
    while (serialAT->available() && index < 511)
    {
        buffer[index++] = serialAT->read();
    }
    buffer[index] = '\0';
    Serial.println(("Received: " + std::string(buffer)).c_str());
    return std::string(buffer);
}
