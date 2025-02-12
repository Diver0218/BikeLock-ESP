#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "dummyLock.h"
#include "dummyMotor.h"
#include "TokenCallbacks.h"

#include "GPS.h"
#include "iGPS_Module.h"
#include "Internet.h"
#include "dummyGPS_Module.h"
#include "WLAN.h"

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

void bluetoothComponent(void* parameter);
void gpsComponent(void* parameter);

Internet *wlan = new WLAN("Das gelobte LAN", "joxmag-qAxrad-zimwi2");

void setup() {
    Serial.begin(115200);

    bluetoothComponent(NULL);
    gpsComponent(NULL);

}

void loop() {
}

void bluetoothComponent (void* parameter)
{
    iMotor *lockMotor = new dummyMotor();
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new dummyLock(lockMotor, safetyMotor);

    Bluetooth *bluetooth = new Bluetooth("SmartLock", lock);

    std::string auth_url = "http://192.168.178.49:3498/LockAuth/";

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();
    BLECharacteristicCallbacks *tokenCallbacks = new TokenCallbacks(wlan, auth_url);

    bluetooth->initialize();
    bluetooth->createServer(serverCallbacks);
    bluetooth->createService(tokenCallbacks);
    bluetooth->startAdvertising();
}

void gpsComponent(void* parameter)
{
    iGPS_Module *gps_module = new dummyGPS_Module();

    std::string gps_url = "http://192.168.178.49:3498/GPSData/";

    GPS *gps = new GPS(gps_url, gps_module, wlan);
    gps->uploadGPS(gps->readGPS());
}