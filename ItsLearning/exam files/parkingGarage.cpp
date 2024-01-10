#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define PARKINGPLACES 3
#define CARS 5

HANDLE parkSem, road1Mutex, road2Mutex, screenMutex;

void car_body(void* vptr)
{
	unsigned int car_no = (unsigned int)vptr, i;
	for (EVER)
	{	
		WaitForSingleObject(screenMutex, INFINITE);
		  for (i = 0; i < car_no - 1; i++) printf("            ");
		  printf(" car %d traffic\n",car_no);
		ReleaseMutex(screenMutex);
		WaitForSingleObject(parkSem, INFINITE);
		if (WaitForSingleObject(road1Mutex, 0) == WAIT_TIMEOUT )
		{
			WaitForSingleObject(road2Mutex, INFINITE);
			  WaitForSingleObject(screenMutex, INFINITE);
			    for (i = 0; i < car_no - 1; i++) printf("            ");
			    printf(" car %d parking road 2\n", car_no);
			  ReleaseMutex(screenMutex);
			ReleaseMutex(road2Mutex);
		}
		else
		{
			WaitForSingleObject(screenMutex, INFINITE);
			  for (i = 0; i < car_no - 1; i++) printf("            ");
			  printf(" car %d parking road 1\n", car_no);
			ReleaseMutex(screenMutex);
		  ReleaseMutex(road1Mutex);
		}
		  WaitForSingleObject(screenMutex, INFINITE);
		    for (i = 0; i < car_no - 1; i++) printf("            ");
		    printf(" car %d parked\n", car_no);
		  ReleaseMutex(screenMutex);
		Sleep(1000 * car_no);
		if ( car_no % 2 == 0)
		{ 
		  WaitForSingleObject(road1Mutex, INFINITE);
			WaitForSingleObject(screenMutex, INFINITE);
			   for (i = 0; i < car_no - 1; i++) printf("            ");
			   printf(" car %d parking road 1\n", car_no);
			ReleaseMutex(screenMutex);
		  ReleaseMutex(road1Mutex);
		}
		else
		{
			WaitForSingleObject(road2Mutex, INFINITE);
			  WaitForSingleObject(screenMutex, INFINITE);
			    for (i = 0; i < car_no - 1; i++) printf("            ");
			    printf(" car %d parking road 2\n", car_no);
			  ReleaseMutex(screenMutex);
			ReleaseMutex(road2Mutex);
		}
		ReleaseSemaphore(parkSem, 1, NULL);
		
		Sleep( car_no);
	}
}


void main( void )
{   
	
	HANDLE car_thread[5];

	road1Mutex = CreateMutex(NULL,FALSE,NULL);
	road2Mutex = CreateMutex(NULL, FALSE, NULL);
	screenMutex = CreateMutex(NULL, FALSE, NULL);
	parkSem = CreateSemaphore(NULL,PARKINGPLACES,PARKINGPLACES,NULL);
	
	for (int i = 0; i < CARS; i++)
	{
		car_thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)car_body, (LPVOID)(i+1), 0, NULL);
	}
	
	
    WaitForMultipleObjects(5,car_thread,true,INFINITE);

}
