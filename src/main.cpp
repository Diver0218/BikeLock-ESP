#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "dummyLock.h"
#include "dummyMotor.h"

std::map<uint16_t, conn_status_t> connectedDevices;

void bluetoothComponent(void* paarameter);

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

    Bluetooth *bluetooth = new Bluetooth("SmartLock", lock);

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();

    bluetooth->initialize();
    bluetooth->createServer();
    bluetooth->createService();
    bluetooth->setCallbacks(serverCallbacks);
    bluetooth->startAdvertising();

    while (true) {
        if (bluetooth->isConnected()) {
            bluetooth->isValid();
        }
    }
}