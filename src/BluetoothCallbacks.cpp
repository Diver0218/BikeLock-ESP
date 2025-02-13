#include "BluetoothCallbacks.h"

void BluetoothCallbacks::onConnect(BLEServer *server) {
    bluetoothExecuting = true;
    Serial.println("Bluetooth Connected");
}

void BluetoothCallbacks::onDisconnect(BLEServer *server) {
    Serial.println("Bluetooth Disconnected");
    bluetoothExecuting = false;
}