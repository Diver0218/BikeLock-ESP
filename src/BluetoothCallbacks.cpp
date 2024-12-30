#include "BluetoothCallbacks.h"

void BluetoothCallbacks::onConnect(BLEServer *server) {
    Serial.println("Connected");
    connectedDevices = server->getPeerDevices(true);
}

void BluetoothCallbacks::onDisconnect(BLEServer *server) {
    Serial.println("Disconnected");
}