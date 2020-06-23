
#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
TaskHandle_t myTask1Handle = NULL;
TaskHandle_t myTask2Handle = NULL;

SemaphoreHandle_t xSemaphore = NULL;

TaskHandle_t ISR = NULL;

#define CONFIG_LED_PIN 2
#define ESP_INTR_FLAG_DEFAULT 0
#define CONFIG_BUTTON_PIN 0
void IRAM_ATTR button_isr_handler(void* arg) {
  
	xSemaphoreGiveFromISR(xSemaphore, NULL);
}


 void task1(void *arg)
{


 while(1){  

   vTaskDelay(pdMS_TO_TICKS(5000));
//printf("hello from task 1 [%d]\n",xTaskGetTickCount());
//   xSemaphoreGive(xSemaphore);

 }
}

 void task2(void *arg)
{

while(1){
vTaskDelay(pdMS_TO_TICKS(3000));
printf("waiting the button!\n");
		if(xSemaphoreTake(xSemaphore,portMAX_DELAY)) {
			printf("got message! [%d] \n",xTaskGetTickCount());

		}
 }
}

void app_main()
{

  gpio_pad_select_gpio(CONFIG_BUTTON_PIN);
  gpio_pad_select_gpio(CONFIG_LED_PIN);
  
  // set the correct direction
  gpio_set_direction(CONFIG_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_LED_PIN, GPIO_MODE_OUTPUT);
  
  // enable interrupt on falling (1->0) edge for button pin
  gpio_set_intr_type(CONFIG_BUTTON_PIN, GPIO_INTR_NEGEDGE);

  
  //Install the driver’s GPIO ISR handler service, which allows per-pin GPIO interrupt handlers.
  // install ISR service with default configuration
  gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  
  // attach the interrupt service routine
  gpio_isr_handler_add(CONFIG_BUTTON_PIN, button_isr_handler, NULL);


xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(task1, "task1", 4096, NULL, 10, &myTask1Handle);
   xTaskCreatePinnedToCore(task2, "task2", 4096, NULL, 10, &myTask2Handle,1);
}
