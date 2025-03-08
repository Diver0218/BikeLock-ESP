#include "Cellular.h"
#include "globals.h"

Cellular::Cellular(std::string pin, std::string apn, std::string user, std::string password, int rx, int tx) : pin(pin), apn(apn), user(user), pass(password), rx(rx), tx(tx)
{   
}

void Cellular::connect()
{
    xSemaphoreTake(gsm_semaphore, portMAX_DELAY);

    // Initialize Serial2 (UART2) on ESP32 (RX=16, TX=17)
    modem = new HardwareSerial(2);
    modem->begin(9600, SERIAL_8N1, rx, tx);

    sendSerial("AT+CFUN=1,1", 4000);                        // Modem einschalten
    sendSerial("AT+CPIN=\"" + pin + "\"");                  // PIN setzen
    sendSerial("AT+CGATT=1");                               // GPRS-Anmeldung erzwingen
    sendSerial("AT+SAPBR=0,1");                             // GPRS-Verbindung löschen   
    sendSerial("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");        // GPRS-Verbindungstyp setzen
    sendSerial(("AT+SAPBR=3,1,\"APN\",\"" + apn + "\"").c_str());     // APN konfigurieren
    sendSerial(("AT+SAPBR=3,1,\"USER\",\"" + user + "\"").c_str());   // Benutzername konfigurieren
    sendSerial(("AT+SAPBR=3,1,\"PWD\",\"" + pass + "\"").c_str());    // Passwort konfigurieren
    sendSerial("AT+SAPBR=1,1", 2000);                       // GPRS-Verbindung öffnen
    sendSerial("AT+SAPBR=2,1", 2000);                       // Verbindung prüfen

    Serial.println("Cellular connected to the internet");
}

void Cellular::disconnect()
{
    sendSerial("AT+SAPBR=0,1");                             // GPRS-Verbindung löschen
    sendSerial("AT+CGATT=0");                               // GPRS-Anmeldung beenden
    sendSerial("AT+CFUN=0");                            // Modem in den Schlafmodus versetzen
    delete modem;
    xSemaphoreGive(gsm_semaphore);
}

int Cellular::get(std::string url)
{
    sendSerial("AT+HTTPTERM");                    // Falls alte Session hängt
    sendSerial("AT+HTTPINIT");                              // HTTP-Session initialisieren
    sendSerial("AT+HTTPPARA=\"CID\",1");                    // Verbindungskontext setzen
    sendSerial(("AT+HTTPPARA=\"URL\",\"" + url + "\"").c_str()); // URL setzen
    sendSerial("AT+HTTPACTION=0");                          // HTTP-GET ausführen
    std::string response = sendSerial("AT+HTTPREAD");       // Antwort lesen
    sendSerial("AT+HTTPTERM");                              // HTTP-Session beenden


    //+HTTPACTION: 1,200,28
    size_t coma = response.find(",");
    response = response.substr(coma + 1, 3);

    int returncode = atoi(response.c_str()); 
    Serial.println(("HTTP GET Response: " + std::to_string(returncode)).c_str());
    return returncode;
}

int Cellular::post(std::string url, std::string payload)
{
    sendSerial("AT+HTTPTERM");                              // Falls alte Session hängt
    sendSerial("AT+HTTPINIT");                              // HTTP-Session initialisieren
    sendSerial("AT+HTTPPARA=\"CID\",1");                    // Verbindungskontext setzen
    sendSerial(("AT+HTTPPARA=\"URL\",\"" + url + "\"").c_str());   // URL setzen
    sendSerial("AT+HTTPPARA=\"CONTENT\",\"application/json\"");    // Content-Type setzen
    sendSerial(("AT+HTTPDATA=" + std::to_string(payload.length()) + ",10000").c_str()); // Daten senden
    sendSerial(payload.c_str());
    sendSerial("AT+HTTPACTION=1");                          // HTTP-GET ausführen
    std::string response = sendSerial("AT+HTTPREAD");       // Antwort lesen
    sendSerial("AT+HTTPTERM");                              // HTTP-Session beenden

    size_t coma = response.find(",");
    response = response.substr(coma + 1, 3);
    
    int returncode = atoi(response.c_str()); 
    Serial.println(("HTTP GET Response: " + std::to_string(returncode)).c_str());
    return returncode;
}

std::string Cellular::sendSerial(std::string message, int taskdelay)
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
