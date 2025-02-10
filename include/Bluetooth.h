#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_sleep.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <map>
#include <stdint.h>

#include "iLock.h"
#include "BluetoothCallbacks.h"

#define SERVICE_UUID BLEUUID("ae578e1d-5457-46e4-9eda-ce03a4534896")            // Custom UUID for Custom Service
#define CHARACTERISTIC_UUID BLEUUID("0f3bd0a0-7bc0-448f-bd02-793fb412b4af")          // Custom UUID for Custom Characteristic

class Bluetooth {

private:
    const char *deviceName;
    BLEServer *server;
    BLEService *service;
    BLECharacteristic *characteristic;
    BLEAdvertising *advertising;
    iLock *lock;

public:
    Bluetooth(const char *deviceName, iLock *lock);
    void initialize();
    void createServer(BLEServerCallbacks *serverCallbacks);
    void createService(BLECharacteristicCallbacks *characteristicCallbacks);
    void startAdvertising();
    bool isConnected();
    bool isValid();
    void toggleLock();
};