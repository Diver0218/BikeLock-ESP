#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "dummyLock.h"
#include "dummyMotor.h"

void bluetoothComponent(void* paarameter);

std::map<uint16_t, conn_status_t> connectedDevices;

void setup() {
  Serial.begin(115200);
  bluetoothComponent(NULL);
}

void loop() {
}

void bluetoothComponent (void* paarameter)
{
    iMotor *lockMotor = new dummyMotor();
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new dummyLock(lockMotor, safetyMotor);

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();

    Bluetooth *bluetooth = new Bluetooth("SmartLock", lock);
    bluetooth->initialize();
    bluetooth->createServer();
    bluetooth->createService();
    bluetooth->setCallbacks();
    bluetooth->startAdvertising();

    while (true) {
        if (bluetooth->isConnected()) {
            bluetooth->isValid();
        }
    }
}