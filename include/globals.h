#ifndef GMS_SEMAPHORE
#define GMS_SEMAPHORE

#include <freertos/semphr.h>
extern SemaphoreHandle_t gsm_semaphore;
extern bool bluetoothExecuting;
extern bool CallbackExecuting;

#endif // GMS_SEMAPHORE