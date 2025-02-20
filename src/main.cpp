#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "globals.h"
#include "privateData.h"

SemaphoreHandle_t gsm_semaphore;

RTC_DATA_ATTR int GPSCount = 50;
int readyToSleep = 0;
bool bluetoothExecuting = false;

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

#define GSM_RX 0
#define GSM_TX 0

#define STEPPER_PIN_1 32
#define STEPPER_PIN_2 33
#define STEPPER_PIN_3 25
#define STEPPER_PIN_4 26

#define STEPPER_RESOLUTION 2048
#define STEPS_TO_LOCK 2048
#define STEPPER_SPEED 10

#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "Lock.h"
#include "Stepper_Motor.h"
#include "dummyMotor.h"
#include "TokenCallbacks.h"

#include "GPS.h"
#include "iGPS_Module.h"
#include "Internet.h"
#include "dummyGPS_Module.h"
#include "WLAN.h"

void bluetoothComponent(void* parameter);
void gpsComponent(void* parameter);
void checkSleep();
bool checkGPSExecution();

Internet *wlan = new WLAN(wifiName, wifiPassword);

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

    if (checkGPSExecution())
    {
        BaseType_t gpsTask = xTaskCreatePinnedToCore(gpsComponent, "GPSTask", 8192, NULL, 1, NULL, 1);
        if (gpsTask != pdPASS) {
            Serial.println("Failed to create GPS task");
        } else {
            Serial.println("GPS task created successfully");
        }
    }
    else
    {
        Serial.println("GPS task not created");
        readyToSleep++;
    }
}

void loop() {
    // Serial.printf("Free heap: %u\n", esp_get_free_heap_size());
    // vTaskDelay(100 / portTICK_PERIOD_MS);
}

void bluetoothComponent(void* parameter) {
    Serial.printf("BL: Free heap: %u\n", esp_get_free_heap_size());
    Serial.println("\n-------------------------------------\nBluetooth component started\n-------------------------------------\n");

    iMotor *lockMotor = new Stepper_Motor(STEPPER_RESOLUTION, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4, STEPPER_SPEED);
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new Lock(lockMotor, safetyMotor, STEPS_TO_LOCK);

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

    
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    Serial.println("Bluetooth component finished");

    if (bluetoothExecuting)
    {
        Serial.println("Bluetooth still executing, waiting for it to finish");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    checkSleep();
    vTaskDelete(NULL);
}

void gpsComponent(void* parameter) {
    Serial.printf("GPS: Free heap: %u\n", esp_get_free_heap_size());
    Serial.println("\n-------------------------------------\nGPS component started\n-------------------------------------\n");

    iGPS_Module *gps_module = new dummyGPS_Module();

    std::string gps_url = "http://192.168.178.49:3498/GPSData/";

    GPS *gps = new GPS(gps_url, gps_module, wlan);
    Serial.println("Reading GPS data");
    gps->uploadGPS(gps->readGPS());

    Serial.println("Cleaning up GPS resources");
    delete gps;
    delete gps_module;

    Serial.println("GPS component finished");

    checkSleep();
    vTaskDelete(NULL);
}

void checkSleep() {
    Serial.println("Checking if ready to sleep...");
    readyToSleep++;
    if (readyToSleep == 2)
    {
        readyToSleep = 0;
        Serial.println("\n-------------------------------------\nGoing to sleep\n-------------------------------------\n");

        esp_sleep_enable_timer_wakeup(7 * 1000000);
        esp_deep_sleep_start();
    }
    else
    {
        Serial.print("Not ready to sleep yet, counter at: ");
        Serial.print(readyToSleep);
        Serial.println("   needs to be 2");
    }
}

bool checkGPSExecution()
{
    if (GPSCount == 50)
    {
        GPSCount = 0;
        return true;
    }
    else
    {
        GPSCount++;
        return false;
    }
}
