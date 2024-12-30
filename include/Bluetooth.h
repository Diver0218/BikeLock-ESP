#include <BLEServer.h>
#include "iLock.h"

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
    void setCallbacks();
    void startAdvertising();
    bool isConnected();
    bool isValid();
    void toggleLock();
};