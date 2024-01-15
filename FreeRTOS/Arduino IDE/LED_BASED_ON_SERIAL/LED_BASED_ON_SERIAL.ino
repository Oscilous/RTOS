#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Pins
static const int led_pin = LED_BUILTIN;

int time = 100;

static TaskHandle_t hTask_1 = NULL;
static TaskHandle_t hTask_2 = NULL;

void funcTask_1(void *parameter){
  
}

void funcTask_2(void *parameter){
  while (1){
    Serial.print("*");
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(300);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // put your setup code here, to run once:
  Serial.println();
  Serial.println("Connected");
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
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i =0; i < 3; i++){
    vTaskSuspend(hTask_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    vTaskResume(hTask_2);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  if (hTask_1 != NULL){
    vTaskDelete(hTask_1);
    hTask_1 = NULL;
  }
}
