#include "Bluetooth.h"
#include <BLEDevice.h>
#include <BLE2902.h>

Bluetooth::Bluetooth(const char *deviceName, iLock *lock) : deviceName(deviceName), lock(lock) {
    server = nullptr;
    service = nullptr;
    characteristic = nullptr;
    advertising = nullptr;
}

void Bluetooth::initialize() {
    BLEDevice::init(deviceName);
}

void Bluetooth::createServer(BLEServerCallbacks *serverCallbacks) {
    server = BLEDevice::createServer();
    server->setCallbacks(serverCallbacks);
}

void Bluetooth::createService(BLECharacteristicCallbacks *characteristicCallbacks) {
    service = server->createService(SERVICE_UUID);
    characteristic = new BLECharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_WRITE
    );
    service->addCharacteristic(characteristic);
    auto descriptor = new BLE2902();
    characteristic->addDescriptor(descriptor);
    characteristic->setCallbacks(characteristicCallbacks);
    service->start();
}

void Bluetooth::startAdvertising() {
    advertising = BLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->setMinPreferred(0x06);
    advertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Advertising started");
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