#include "Arduino.h"

#define configUSE_PREEMPTION 1
#define configUSE_TIME_SLICING 1


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t id_queue_len = 1;
static QueueHandle_t id_queue = NULL;

static TaskHandle_t hServer = NULL;
static TaskHandle_t hClient1, hClient2, hClient3 = NULL;

char uniqueCharClient1 = ' ';
char uniqueCharClient2 = ' ';
char uniqueCharClient3 = ' ';

void funcClient(void *parameter){
  char* client_variable_ptr = (char*)parameter;
  while (1) {
    if (xQueueSend(id_queue, &client_variable_ptr, portMAX_DELAY) != pdTRUE) {
      Serial.println("Queue send failed");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void funcServer(void *parameter){
  char* own_ptr;
  char nextUniqueChar = 'a';
  while (1){
      if (xQueueReceive(id_queue, (void *)&own_ptr, 0) == pdTRUE){
        *own_ptr = nextUniqueChar++;
        Serial.print("Char assigned:");
        Serial.println(*own_ptr);
      }
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}


void setup() {
  Serial.begin(9600);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Connected");
  
  id_queue = xQueueCreate(id_queue_len, sizeof(char*));


  xTaskCreatePinnedToCore(funcClient, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        &uniqueCharClient1, //argument
                        1, //Task priority
                        &hClient1, // Task handle
                        app_cpu); //for one core only 
  xTaskCreatePinnedToCore(funcClient, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        &uniqueCharClient2, //argument
                        1, //Task priority
                        &hClient2, // Task handle
                        app_cpu); //for one core only 
  xTaskCreatePinnedToCore(funcClient, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        &uniqueCharClient3, //argument
                        1, //Task priority
                        &hClient3, // Task handle
                        app_cpu); //for one core only 
  xTaskCreatePinnedToCore(funcServer, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        NULL, //argument
                        2, //Task priority
                        &hServer, // Task handle
                        app_cpu); //for one core only 
  vTaskDelete(NULL);
  vTaskStartScheduler();
  for (;;);
}

void loop() {
  //Never runs
}