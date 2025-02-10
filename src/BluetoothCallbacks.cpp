#include "BluetoothCallbacks.h"

void BluetoothCallbacks::onConnect(BLEServer *server) {
    Serial.println("Connected");
}

void BluetoothCallbacks::onDisconnect(BLEServer *server) {
    Serial.println("Disconnected");
}