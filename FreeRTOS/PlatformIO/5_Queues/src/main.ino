#include "Arduino.h"

#define configUSE_PREEMPTION 1
#define configUSE_TIME_SLICING 1


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t msg_queue_len = 5;
static QueueHandle_t msg_queue = NULL;

static TaskHandle_t hprintMessage = NULL;
static TaskHandle_t hTask_2 = NULL;

void increment(void *parameter){
  while (1){
    static int num = 0;
    if (xQueueSend(msg_queue, (void *)&num, 10) != pdTRUE){
      Serial.println("Queue is full");
    }
    num++;
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void printMessage(void *parameter){
  while (1){
    int item;
    for (;;){
      if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE){
        Serial.print("Message: ");
        Serial.println(item);
      }
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}


void setup() {
  Serial.begin(9600);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Connected");
  
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));


  xTaskCreatePinnedToCore(printMessage, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        NULL, //argument
                        1, //Task priority
                        &hprintMessage, // Task handle
                        app_cpu); //for one core only 
  xTaskCreatePinnedToCore(increment, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        NULL, //argument
                        2, //Task priority
                        &hTask_2, // Task handle
                        app_cpu); //for one core only 
  vTaskDelete(NULL);
  vTaskStartScheduler();
  for (;;);
}

void loop() {
  //Never runs
}