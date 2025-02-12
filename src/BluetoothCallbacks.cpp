#include "BluetoothCallbacks.h"

void BluetoothCallbacks::onConnect(BLEServer *server) {
    Serial.println("Bluetooth Connected");
}

void BluetoothCallbacks::onDisconnect(BLEServer *server) {
    Serial.println("Bluetooth Disconnected");
}