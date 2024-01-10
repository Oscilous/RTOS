
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

enum uniqueReq  { INT_WANTED, CHAR_WANTED };
struct Req { enum uniqueReq req_kind; void* unique_ptr; };
static xQueueHandle uniqueReqQueue;

static void uniqueIDserver(void *vptr );
static void intClient(void *vptr );
static void char1Client(void *vptr );
static void char2Client(void *vptr );

static xSemaphoreHandle screenMutex;

static int uniqueInt;
static char unique1Char;
static char unique2Char;


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

      LCD_DisplayStringLine(LINE(1), (uint8_t*)" hello unique 1 ");
      screenMutex = xSemaphoreCreateMutex();
      uniqueReqQueue =  xQueueCreate(1, sizeof(struct Req));
	  xTaskCreate( uniqueIDserver,
				 ( signed char * ) "uniqueIDserver",
				configMINIMAL_STACK_SIZE,
				NULL,
				2,
				NULL );
	xTaskCreate( intClient,
				( signed char * ) "intClient",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );
	xTaskCreate( char1Client,
				( signed char * ) "char1Client",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );
	xTaskCreate( char2Client,
				( signed char * ) "char2Client",
				configMINIMAL_STACK_SIZE,
				NULL,
				1,
				NULL );

	vTaskStartScheduler();

	for( ;; );
}

static void uniqueIDserver( void *pvParameters )
{
	struct Req req;
	int* uniqueIntPtr;
	char* uniqueCharPtr;
    int i = 1;
    char ch ='a';
    char chi[2] = "1";
    char chch[2] = "a";
    LCD_DisplayStringLine(LINE(3), (uint8_t*)" Unique ID ");
	for( ;; )
	{
	   xQueueReceive(uniqueReqQueue, &req, portMAX_DELAY);
	   if ( req.req_kind == INT_WANTED )
		  {
		    uniqueIntPtr = (int*)req.unique_ptr;
		    *uniqueIntPtr = i;
		    xSemaphoreTake(screenMutex,portMAX_DELAY);
		      LCD_DisplayStringLine(LINE(5), (uint8_t*) &chi);
		    xSemaphoreGive(screenMutex);
		    i++;
		    chi[0] = '0' + i;
		    if ( i == 10 )
		 	   {
		 		   vTaskDelay(portMAX_DELAY);
		 	   }
		  }
	   if ( req.req_kind == CHAR_WANTED )
	  	  {
		    uniqueCharPtr = (char*)req.unique_ptr;
	  		*uniqueCharPtr = ch;
	  		xSemaphoreTake(screenMutex,portMAX_DELAY);
	  		   LCD_DisplayStringLine(LINE(4), (uint8_t*) &chch);
	  		xSemaphoreGive(screenMutex);
	  		ch = ch + 1;
	  		chch[0] = ch;
	  		if ( ch ==  'z'+1)
	     	   {
	  	    	   vTaskDelay(portMAX_DELAY);
	  		   }
	  	   }
	}
}

static void intClient( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType two_sec = ( 2000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();
	struct Req req;
	req.req_kind = INT_WANTED;
	req.unique_ptr = &uniqueInt;
    char chi[2] = "0";
    LCD_DisplayStringLine(LINE(6), (uint8_t*)" intClient: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,&req,portMAX_DELAY);
		xSemaphoreTake(screenMutex,portMAX_DELAY);
		  chi[0] = '0' + uniqueInt;
		  LCD_DisplayStringLine(LINE(7), (uint8_t*) &chi);
		xSemaphoreGive(screenMutex);
		vTaskDelayUntil( &xNextWakeTime, two_sec*3 );
	}
}

static void char1Client( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType one_sec = ( 1000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();
	struct Req req;
	req.req_kind = CHAR_WANTED;
	req.unique_ptr = &unique1Char;
    char chi[2] = "a";
    LCD_DisplayStringLine(LINE(8), (uint8_t*)"char1Client: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,&req,portMAX_DELAY);
		xSemaphoreTake(screenMutex,portMAX_DELAY);
		  chi[0] = unique1Char;
		  LCD_DisplayStringLine(LINE(9), (uint8_t*) &chi);
		xSemaphoreGive(screenMutex);
		vTaskDelayUntil( &xNextWakeTime, one_sec );
	}
}

static void char2Client( void *pvParameters )
{
    portTickType xNextWakeTime;
    portTickType two_sec = ( 2000 / portTICK_RATE_MS );
	xNextWakeTime = xTaskGetTickCount();
	struct Req req;
	req.req_kind = CHAR_WANTED;
	req.unique_ptr = &unique2Char;
    char chi[2] = "a";
    LCD_DisplayStringLine(LINE(10), (uint8_t*)"char2Client: ");
	for( ;; )
	{
		xQueueSend(uniqueReqQueue,&req,portMAX_DELAY);
		xSemaphoreTake(screenMutex,portMAX_DELAY);
		  chi[0] = unique2Char;
		  LCD_DisplayStringLine(LINE(11), (uint8_t*) &chi);
		xSemaphoreGive(screenMutex);
		vTaskDelayUntil( &xNextWakeTime, two_sec );
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
