#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_sleep.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <map>
#include <stdint.h>

#include "iLock.h"
#include "BluetoothCallbacks.h"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class Bluetooth {

private:
    const char *deviceName;
    BLEServerCallbacks *serverCallbacks;
    BLEServer *server;
    BLEService *service;
    BLECharacteristic *characteristic;
    BLEAdvertising *advertising;
    iLock *lock;

public:
    Bluetooth(const char *deviceName, iLock *lock);
    ~Bluetooth();
    void initialize();
    void createServer();
    void createService();
    void setCallbacks();
    void startAdvertising();
    bool isConnected();
    bool isValid();
    void toggleLock();
};