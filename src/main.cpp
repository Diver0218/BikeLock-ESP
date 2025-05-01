#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <esp_task_wdt.h>

#include "globals.h"
#include "privateData.h"

SemaphoreHandle_t gsm_semaphore;

#define SKIP_BEGINNING_GPS

#ifdef SKIP_BEGINNING_GPS
    RTC_DATA_ATTR int GPSCount = 0;
#else
    RTC_DATA_ATTR int GPSCount = 50;
#endif

int readyToSleep = 0;
bool bluetoothExecuting = false;
bool CallbackExecuting = false;
RTC_DATA_ATTR bool isLocked_global = false;

#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024

#define GSM_RX 16
#define GSM_TX 17

#define STEPPER_PIN_1 32
#define STEPPER_PIN_2 33
#define STEPPER_PIN_3 25
#define STEPPER_PIN_4 26

#define STEPPER_RESOLUTION 4096
#define STEPS_TO_LOCK 6039
#define STEPPER_SPEED 10

#define WDT_TIMEOUT 60 //Sekunden

#define URL "jamin.chickenkiller.com"
// #define URL "79.249.91.73"
#define AUTH_URL "/LockAuth/"
#define GPS_URL "/GPSData/"


#include <Arduino.h>
#include <map>
#include <BLEServer.h>

#include "Bluetooth.h"
#include "Lock.h"
#include "Stepper_Motor.h"
#include "dummyMotor.h"
#include "TokenCallbacks.h"

#include "Location_component.h"
#include "Location.h"
#include "Cellular_Location.h"
#include "Internet.h"
#include "dummyGPS_Module.h"
#include "WLAN.h"
#include "Cellular.h"

void bluetoothComponent(void* parameter);
void gpsComponent(void* parameter);
void checkSleep();
bool checkGPSExecution();

#define USE_WIFI
#ifdef USE_WIFI
    Internet *internet = new WLAN(wifiName, wifiPassword);
#else
    Internet *internet = new Cellular(pin1, apn, user, pass, GSM_RX, GSM_TX);
#endif

void setup() {
    Serial.begin(115200);
    Serial.println("Setup started");

    

    esp_task_wdt_init(WDT_TIMEOUT, true);

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

    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();

    iMotor *lockMotor = new Stepper_Motor(STEPPER_RESOLUTION, STEPPER_PIN_1, STEPPER_PIN_2, STEPPER_PIN_3, STEPPER_PIN_4, STEPPER_SPEED);
    iMotor *safetyMotor = new dummyMotor();
    iLock *lock = new Lock(lockMotor, safetyMotor, STEPS_TO_LOCK);

    Bluetooth *bluetooth = new Bluetooth("SmartLock");

    std::string auth_url = std::string(URL) + std::string(AUTH_URL);

    BLEServerCallbacks *serverCallbacks = new BluetoothCallbacks();
    BLECharacteristicCallbacks *tokenCallbacks = new TokenCallbacks(internet, auth_url, lock);

    Serial.println("Initializing Bluetooth");
    bluetooth->initialize();
    Serial.println("Creating Bluetooth server");
    bluetooth->createServer(serverCallbacks);
    Serial.println("Creating Bluetooth service");
    bluetooth->createService(tokenCallbacks);
    Serial.println("Starting Bluetooth advertising");
    bluetooth->startAdvertising();

    
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    Serial.println("Bluetooth component finished");

    while (bluetoothExecuting || CallbackExecuting)
    {
        Serial.println("Bluetooth still executing, waiting for it to finish");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    esp_task_wdt_reset();
    checkSleep();
    vTaskDelete(NULL);
}

void gpsComponent(void* parameter) {
    Serial.printf("GPS: Free heap: %u\n", esp_get_free_heap_size());
    Serial.println("\n-------------------------------------\nGPS component started\n-------------------------------------\n");

    esp_task_wdt_add(NULL);
    esp_task_wdt_reset();

    Location *location_module = new Cellular_location(pin1, apn, user, pass, GSM_RX, GSM_TX);
    // iGPS_Module *gps_module = new dummyGPS_Module();

    std::string gps_url = std::string(URL) + std::string(GPS_URL);

    Location_component *location = new Location_component(gps_url, location_module, internet);
    Serial.println("Reading GPS data");
    location->uploadGPS(location->readGPS());

    Serial.println("Cleaning up GPS resources");
    delete location;
    delete location_module;

    Serial.println("GPS component finished");

    esp_task_wdt_reset();
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
