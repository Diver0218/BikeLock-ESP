#ifndef GMS_SEMAPHORE
#define GMS_SEMAPHORE

#include <freertos/semphr.h>
extern SemaphoreHandle_t gsm_semaphore;
extern bool bluetoothExecuting;

#endif // GMS_SEMAPHORE