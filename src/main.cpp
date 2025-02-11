#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "dummyLock.h"
#include "dummyMotor.h"
#include "TokenCallbacks.h"

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

void bluetoothComponent(void* paarameter);

void setup() {
    Serial.begin(115200);
    // bluetoothComponent(NULL);
    iMotor *lockMotor = new dummyMotor();
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new dummyLock(lockMotor, safetyMotor);

    Bluetooth *bluetooth = new Bluetooth("SmartLock", lock);

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();
    BLECharacteristicCallbacks *tokenCallbacks = new TokenCallbacks();

    bluetooth->initialize();
    bluetooth->createServer(serverCallbacks);
    bluetooth->createService(tokenCallbacks);
    bluetooth->startAdvertising();
}

void loop() {

    delay(10);
}

void bluetoothComponent (void* paarameter)
{
    // iMotor *lockMotor = new dummyMotor();
    // iMotor *safetyMotor = new dummyMotor();
    // iLock *lock = new dummyLock(lockMotor, safetyMotor);

    // Bluetooth *bluetooth = new Bluetooth("SmartLock", lock);

    // BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();

    // bluetooth->initialize();
    // bluetooth->createServer();
    // bluetooth->createService();
    // bluetooth->setCallbacks(serverCallbacks);
    // bluetooth->startAdvertising();

    // while (true) {
    //     Serial.println("Checking connection");
    //     if (bluetooth->isConnected()) {            
    //         bluetooth->isValid();
    //     }
    // }
}