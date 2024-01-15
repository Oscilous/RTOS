#include "Arduino.h"

#define configUSE_PREEMPTION 1
#define configUSE_TIME_SLICING 1


#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Pins
static const int led_pin = LED_BUILTIN;

int led_delay = 1000;

static TaskHandle_t hTask_1 = NULL;
static TaskHandle_t hTask_2 = NULL;

void funcTask_1(void *parameter){
  while (1){
    
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
  }
}

void funcTask_2(void *parameter){
  while (1){
    if (Serial.available() > 0) {
      String data = Serial.readStringUntil('\n');
      if (data.length() > 0) {
        int new_delay = data.toInt();
        if (new_delay > 0) { // Check to avoid 0 or invalid values
          led_delay = new_delay;
          Serial.print("Updated the ms: ");
          Serial.println(led_delay);
        }
      }
    }
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}


void setup() {
  Serial.begin(9600);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // put your setup code here, to run once:
  Serial.println();
  Serial.println("Connected");
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(funcTask_1, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        NULL, //argument
                        1, //Task priority
                        &hTask_1, // Task handle
                        app_cpu); //for one core only 
  xTaskCreatePinnedToCore(funcTask_2, //Function to run
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