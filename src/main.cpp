#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "globals.h"

SemaphoreHandle_t gsm_semaphore;

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "dummyLock.h"
#include "dummyMotor.h"
#include "TokenCallbacks.h"

#include "GPS.h"
#include "iGPS_Module.h"
#include "Internet.h"
#include "dummyGPS_Module.h"
#include "WLAN.h"

void bluetoothComponent(void* parameter);
void gpsComponent(void* parameter);

Internet *wlan = new WLAN("Das gelobte LAN", "joxmag-qAxrad-zimwi2");

void setup() {
    Serial.begin(115200);
    Serial.println("Setup started");

    gsm_semaphore = xSemaphoreCreateMutex();
    if (gsm_semaphore == NULL) {
        Serial.println("Failed to create semaphore");
    } else {
        Serial.println("Semaphore created successfully");
    }

    BaseType_t btTask = xTaskCreatePinnedToCore(bluetoothComponent, "BluetoothTask", 8192, NULL, 1, NULL, 0);
    if (btTask != pdPASS) {
        Serial.println("Failed to create Bluetooth task");
    } else {
        Serial.println("Bluetooth task created successfully");
    }

    BaseType_t gpsTask = xTaskCreatePinnedToCore(gpsComponent, "GPSTask", 8192, NULL, 1, NULL, 1);
    if (gpsTask != pdPASS) {
        Serial.println("Failed to create GPS task");
    } else {
        Serial.println("GPS task created successfully");
    }

    Serial.println("Setup completed");
}

void loop() {
    // Serial.printf("Free heap: %u\n", esp_get_free_heap_size());
    // vTaskDelay(100 / portTICK_PERIOD_MS);
}

void bluetoothComponent(void* parameter) {
    Serial.printf("BL: Free heap: %u\n", esp_get_free_heap_size());
    Serial.println("Bluetooth component started");

    iMotor *lockMotor = new dummyMotor();
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new dummyLock(lockMotor, safetyMotor);

    Bluetooth *bluetooth = new Bluetooth("SmartLock");

    std::string auth_url = "http://192.168.178.49:3498/LockAuth/";

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();
    BLECharacteristicCallbacks *tokenCallbacks = new TokenCallbacks(wlan, auth_url, lock);

    Serial.println("Initializing Bluetooth");
    bluetooth->initialize();
    Serial.println("Creating Bluetooth server");
    bluetooth->createServer(serverCallbacks);
    Serial.println("Creating Bluetooth service");
    bluetooth->createService(tokenCallbacks);
    Serial.println("Starting Bluetooth advertising");
    bluetooth->startAdvertising();

    Serial.println("Cleaning up Bluetooth resources");
    Serial.println("Bluetooth component finished");
}

void gpsComponent(void* parameter) {
    Serial.printf("GPS: Free heap: %u\n", esp_get_free_heap_size());
    Serial.println("GPS component started");

    iGPS_Module *gps_module = new dummyGPS_Module();

    std::string gps_url = "http://192.168.178.49:3498/GPSData/";

    GPS *gps = new GPS(gps_url, gps_module, wlan);
    Serial.println("Reading GPS data");
    gps->uploadGPS(gps->readGPS());

    Serial.println("Cleaning up GPS resources");
    delete gps;
    delete gps_module;

    Serial.println("GPS component finished");
}