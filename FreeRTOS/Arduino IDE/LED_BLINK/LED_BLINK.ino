#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Pins
static const int led_pin = LED_BUILTIN;

void toggleLED(void *parameter){
  while (1){
    digitalWrite(led_pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led_pin, OUTPUT);
  xTaskCreatePinnedToCore(toggleLED, //Function to run
                        NULL, // Name of task
                        1024, //Stack size
                        NULL, //argument
                        1, //Task priority
                        NULL, // Task handle
                        app_cpu); //for one core only 
}

void loop() {
  // put your main code here, to run repeatedly:

}
