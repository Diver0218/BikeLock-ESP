#include "Bluetooth.h"
#include <BLEDevice.h>

Bluetooth::Bluetooth(const char *deviceName, iLock *lock) : deviceName(deviceName), lock(lock) {
    serverCallbacks = nullptr;
    server = nullptr;
    service = nullptr;
    characteristic = nullptr;
    advertising = nullptr;
}

Bluetooth::~Bluetooth() {
    delete serverCallbacks;
    delete server;
    delete service;
    delete characteristic;
    delete advertising;
}

void Bluetooth::initialize() {
    BLEDevice::init(deviceName);
}

void Bluetooth::createServer() {
    server = BLEDevice::createServer();
}

void Bluetooth::createService() {
    service = server->createService(SERVICE_UUID);
    characteristic = service->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE
    );
    service->start();
}

void Bluetooth::setCallbacks() {
    serverCallbacks = new BluetoothCallbacks();
    server->setCallbacks(serverCallbacks);
}

void Bluetooth::startAdvertising() {
    advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->start();
}

bool Bluetooth::isConnected() {
    return server->getConnectedCount() > 0;
}

bool Bluetooth::isValid() {
for (const auto &device : connectedDevices) {
    Serial.println(device.first);
}
    return server != nullptr && service != nullptr && characteristic != nullptr;
}

void Bluetooth::toggleLock() {
    lock->toggleLock();
}