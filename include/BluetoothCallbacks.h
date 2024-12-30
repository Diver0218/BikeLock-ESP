#include <BLEServer.h>
#include <Arduino.h>
#include <map>
#include <stdint.h>

extern std::map<uint16_t, conn_status_t> connectedDevices;

class BluetoothCallbacks : public BLEServerCallbacks
{
    void onConnect(BLEServer *server) override;
    void onDisconnect(BLEServer *server) override;
};