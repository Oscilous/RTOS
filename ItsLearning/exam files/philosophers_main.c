
/* Standard includes. */
#include <stdint.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

#include "stm32f4xx.h"
#include <stdio.h>
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"


/* TODO Add any manufacture supplied header files necessary for CMSIS functions
to be available here. */
//#include "stm32f4xx.h"

/* The rate at which data is sent to the queue, specified in milliseconds, and
converted to ticks using the portTICK_RATE_MS constant. */
#define mainQUEUE_SEND_PERIOD_MS			( 200 / portTICK_RATE_MS )

/* The period of the example software timer, specified in milliseconds, and
converted to ticks using the portTICK_RATE_MS constant. */
#define mainSOFTWARE_TIMER_PERIOD_MS		( 1000 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/*-----------------------------------------------------------*/

/*
 * TODO: Implement this function for any hardware specific clock configuration
 * that was not already performed before main() was called.
 */
static void prvSetupHardware( void );
/*
 * The callback function assigned to the example software timer as described at
 * the top of this file.
 */
//static void vExampleTimerCallback( xTimerHandle xTimer );

/*-----------------------------------------------------------*/

/* The queue used by the queue send and queue receive tasks. */
static xQueueHandle xQueue = NULL;

/* The semaphore (in this case binary) that is used by the FreeRTOS tick hook
 * function and the event semaphore task.
 */
static xSemaphoreHandle xEventSemaphore = NULL;

/* The counters used by the various examples.  The usage is described in the
 * comments at the top of this file.
 */
static volatile uint32_t ulCountOfTimerCallbackExecutions = 0;
static volatile uint32_t ulCountOfItemsReceivedOnQueue = 0;
static volatile uint32_t ulCountOfReceivedSemaphores = 0;
/*
 * The event semaphore task as described at the top of this file.
 */
//static void prvEventSemaphoreTask( void *pvParameters );


/*end of freeRTOS stuff-----------------------------------------------------------*/
//  philosophers begin

#define EVER ;;
#define MS 1000
// 1000 means 1 sec.
portTickType pause = ( MS / portTICK_RATE_MS );





static xQueueHandle rightChopstickQueue;

static void philosopher_1(void *vptr );
static void philosopher_2(void *vptr );
static void philosopher_3(void *vptr );
static void philosopher_4(void *vptr );

static xSemaphoreHandle screenMutex;
static xSemaphoreHandle chopStick_1_Mutex;
static xSemaphoreHandle chopStick_2_Mutex;
static xSemaphoreHandle chopStick_3_Mutex;
static xSemaphoreHandle chopStick_4_Mutex;


int main(void)
{
 // graphical initialization begin
	  LCD_Init();
	  LCD_LayerInit();
	  LTDC_Cmd(ENABLE);
	  LCD_SetLayer(LCD_FOREGROUND_LAYER);
	  LCD_Clear(LCD_COLOR_WHITE);
 // graphical initialization end

 // freeRtos stuff begin
   //   xTimerHandle xExampleSoftwareTimer = NULL;
      prvSetupHardware();
	  xQueue = xQueueCreate(mainQUEUE_LENGTH,sizeof( uint32_t ) );
	  vQueueAddToRegistry( xQueue, ( signed char * ) "MainQueue" );
      vSemaphoreCreateBinary( xEventSemaphore );
      vQueueAddToRegistry( xEventSemaphore, ( signed char * ) "xEventSemaphore" );
 // freeRtos stuff end

      LCD_DisplayStringLine(LINE(1), (uint8_t*)"Dining phil's");
      screenMutex = xSemaphoreCreateMutex();
      chopStick_1_Mutex = xSemaphoreCreateMutex();
      chopStick_2_Mutex = xSemaphoreCreateMutex();
      chopStick_3_Mutex = xSemaphoreCreateMutex();
      chopStick_4_Mutex = xSemaphoreCreateMutex();


  
	  xTaskCreate( philosopher_1,
				 ( signed char * ) "philosopher_1",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );
	  xTaskCreate( philosopher_2,
	  				 ( signed char * ) "philosopher_2",
	  				configMINIMAL_STACK_SIZE,
	  				NULL,
	  				1,
	  				NULL );
	  xTaskCreate( philosopher_3,
	  	  				 ( signed char * ) "philosopher_3",
	  	  				configMINIMAL_STACK_SIZE,
	  	  				NULL,
	  	  				1,
	  	  				NULL );
	  xTaskCreate( philosopher_4,
	  	  				 ( signed char * ) "philosopher_4",
	  	  				configMINIMAL_STACK_SIZE,
	  	  				NULL,
	  	  				1,
	  	  				NULL );
	
	vTaskStartScheduler();

	for( ;; );
}

static void philosopher_1( void *pvParameters )
{
	//portTickType xNextWakeTime;

	for( EVER )
	{
		xSemaphoreTake(chopStick_1_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 has ch1" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreTake(chopStick_2_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 has ch2" );
		    xSemaphoreGive(screenMutex);
		   // vTaskDelayUntil( &xNextWakeTime, pause );
		    vTaskDelay( pause );


		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 eats    " );
		    xSemaphoreGive(screenMutex);
		   // vTaskDelayUntil( &xNextWakeTime, pause );
		    vTaskDelay( pause );

		  xSemaphoreGive(chopStick_2_Mutex);
		     xSemaphoreTake(screenMutex,portMAX_DELAY);
		  		LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 puts ch2" );
		  	 xSemaphoreGive(screenMutex);
		  //	vTaskDelayUntil( &xNextWakeTime, pause );
		  	vTaskDelay( pause );

		 xSemaphoreGive(chopStick_1_Mutex);
		   xSemaphoreTake(screenMutex,portMAX_DELAY);
		  	  LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 puts ch1" );
		   xSemaphoreGive(screenMutex);
		   vTaskDelay( pause );// vTaskDelayUntil( &xNextWakeTime, pause );

		 xSemaphoreTake(screenMutex,portMAX_DELAY);
		   	  LCD_DisplayStringLine(LINE(3),(uint8_t*)"ph1 thinks     " );
		 xSemaphoreGive(screenMutex);
		 vTaskDelay( pause );// vTaskDelayUntil( &xNextWakeTime, pause );


	}
}

static void philosopher_2( void *pvParameters )
{

	for( EVER )
	{
		xSemaphoreTake(chopStick_2_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 has ch2" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreTake(chopStick_3_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 has ch3" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 eats    " );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreGive(chopStick_3_Mutex);
		     xSemaphoreTake(screenMutex,portMAX_DELAY);
		  		LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 puts ch3" );
		  	 xSemaphoreGive(screenMutex);
		  	vTaskDelay( pause );

		 xSemaphoreGive(chopStick_2_Mutex);
		   xSemaphoreTake(screenMutex,portMAX_DELAY);
		  	  LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 puts ch2" );
		   xSemaphoreGive(screenMutex);
		   vTaskDelay( pause );

		 xSemaphoreTake(screenMutex,portMAX_DELAY);
		   	  LCD_DisplayStringLine(LINE(5),(uint8_t*)"ph2 thinks     " );
		 xSemaphoreGive(screenMutex);
		 vTaskDelay( pause );


	}
}

static void philosopher_3( void *pvParameters )
{

	for( EVER )
	{
		xSemaphoreTake(chopStick_3_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 has ch3" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreTake(chopStick_4_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 has ch4" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 eats    " );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreGive(chopStick_4_Mutex);
		     xSemaphoreTake(screenMutex,portMAX_DELAY);
		  		LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 puts ch4" );
		  	 xSemaphoreGive(screenMutex);
		  	vTaskDelay( pause );

		 xSemaphoreGive(chopStick_3_Mutex);
		   xSemaphoreTake(screenMutex,portMAX_DELAY);
		  	  LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 puts ch3" );
		   xSemaphoreGive(screenMutex);
		   vTaskDelay( pause );

		 xSemaphoreTake(screenMutex,portMAX_DELAY);
		   	  LCD_DisplayStringLine(LINE(7),(uint8_t*)"ph3 thinks     " );
		 xSemaphoreGive(screenMutex);
		 vTaskDelay( pause );


	}
}

static void philosopher_4( void *pvParameters )
{

	for( EVER )
	{
		xSemaphoreTake(chopStick_4_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 has ch4" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreTake(chopStick_1_Mutex,portMAX_DELAY);
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 has ch1" );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 eats    " );
		    xSemaphoreGive(screenMutex);
		    vTaskDelay( pause );

		  xSemaphoreGive(chopStick_1_Mutex);
		     xSemaphoreTake(screenMutex,portMAX_DELAY);
		  		LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 puts ch1" );
		  	 xSemaphoreGive(screenMutex);
		  	vTaskDelay( pause );

		 xSemaphoreGive(chopStick_4_Mutex);
		   xSemaphoreTake(screenMutex,portMAX_DELAY);
		  	  LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 puts ch4" );
		   xSemaphoreGive(screenMutex);
		   vTaskDelay( pause );

		 xSemaphoreTake(screenMutex,portMAX_DELAY);
		   	  LCD_DisplayStringLine(LINE(9),(uint8_t*)"ph4 thinks     " );
		 xSemaphoreGive(screenMutex);
		 vTaskDelay( pause );


	}
}

/*-----------------------------------------------------------*/
// rest of freeRtos stuff begin
/*
static void vExampleTimerCallback( xTimerHandle xTimer )
{

	ulCountOfTimerCallbackExecutions++;
}
*/

void vApplicationTickHook( void )
{
portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
static uint32_t ulCount = 0;

	/* The RTOS tick hook function is enabled by setting configUSE_TICK_HOOK to
	1 in FreeRTOSConfig.h.

	"Give" the semaphore on every 500th tick interrupt. */
	ulCount++;
	if( ulCount >= 500UL )
	{
		/* This function is called from an interrupt context (the RTOS tick
		interrupt),	so only ISR safe API functions can be used (those that end
		in "FromISR()".

		xHigherPriorityTaskWoken was initialised to pdFALSE, and will be set to
		pdTRUE by xSemaphoreGiveFromISR() if giving the semaphore unblocked a
		task that has equal or higher priority than the interrupted task.
		http://www.freertos.org/a00124.html */
		xSemaphoreGiveFromISR( xEventSemaphore, &xHigherPriorityTaskWoken );
		ulCount = 0UL;
	}

	/* If xHigherPriorityTaskWoken is pdTRUE then a context switch should
	normally be performed before leaving the interrupt (because during the
	execution of the interrupt a task of equal or higher priority than the
	running task was unblocked).  The syntax required to context switch from
	an interrupt is port dependent, so check the documentation of the port you
	are using.  http://www.freertos.org/a00090.html

	In this case, the function is running in the context of the tick interrupt,
	which will automatically check for the higher priority task to run anyway,
	so no further action is required. */
}
/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
	/* The malloc failed hook is enabled by setting
	configUSE_MALLOC_FAILED_HOOK to 1 in FreeRTOSConfig.h.

	Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( xTaskHandle pxTask, signed char *pcTaskName )
{
	( void ) pcTaskName;
	( void ) pxTask;

	/* Run time stack overflow checking is performed if
	configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.  pxCurrentTCB can be
	inspected in the debugger if the task name passed into this function is
	corrupt. */
	for( ;; );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
volatile size_t xFreeStackSpace;

	/* The idle task hook is enabled by setting configUSE_IDLE_HOOK to 1 in
	FreeRTOSConfig.h.

	This function is called on each cycle of the idle task.  In this case it
	does nothing useful, other than report the amount of FreeRTOS heap that
	remains unallocated. */
	xFreeStackSpace = xPortGetFreeHeapSize();

	if( xFreeStackSpace > 100 )
	{
		/* By now, the kernel has allocated everything it is going to, so
		if there is a lot of heap remaining unallocated then
		the value of configTOTAL_HEAP_SIZE in FreeRTOSConfig.h can be
		reduced accordingly. */
	}
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Ensure all priority bits are assigned as preemption priority bits.
	http://www.freertos.org/RTOS-Cortex-M3-M4.html */
	NVIC_SetPriorityGrouping( 0 );

	/* TODO: Setup the clocks, etc. here, if they were not configured before
	main() was called. */
}


/*
 * Callback used by stm324xg_eval_i2c_ee.c.
 * Refer to stm324xg_eval_i2c_ee.h for more info.
 */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* TODO, implement your code here */
  while (1)
  {
  }
}
