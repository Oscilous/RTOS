#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_CARS 5
#define PARKING_SPOTS 3

HANDLE cars[NO_CARS];
// a - mutex, b - semaphore, c - critical section, d - event
enum Problem {a, b, c, d};
Problem solution = a;
HANDLE hSemaphore;
int numbers[NO_CARS] = {1,2,3,4,5};

void my_thread_body(void* vptr);

int main(void)
{
    for (int i = 0; i < NO_CARS; i++)
	{
		cars[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_body, &numbers[i], 0, NULL);
	}
    hSemaphore = CreateSemaphoreW(NULL, (LONG)PARKING_SPOTS, (LONG)PARKING_SPOTS, NULL);
    if (hSemaphore == NULL) //error handling
    {
        printf("CreateSemaphore error: %d\n", GetLastError());
        return 1;
    }
    WaitForMultipleObjects(NO_CARS, cars, true, INFINITE);

    for(int i=0; i < NO_CARS; i++)
        CloseHandle(cars[i]);
    CloseHandle(hSemaphore);
	return 0;
}

void my_thread_body(void* vptr)
{
	//int thread_no = (int) vptr; nasty
	int thread_no = *((int*)vptr); // civilised way of doing it
	for (EVER) 
	{
        DWORD dwWaitResult; 
            // Try to enter the semaphore gate.
            dwWaitResult = WaitForSingleObject( 
                hSemaphore,   // handle to semaphore
                0L);           // zero-second time-out interval

            switch (dwWaitResult) 
            { 
                // The semaphore object was signaled.
                case WAIT_OBJECT_0: 
                {


                    // Release the semaphore when task is finished
                    if (!ReleaseSemaphore(hSemaphore, 1, NULL)){
                        printf("ReleaseSemaphore error: %d\n", GetLastError());
                    }
                    break; 
                }
                // The semaphore was nonsignaled, so a time-out occurred.
                case WAIT_TIMEOUT:
                {
                    printf("Thread %d: wait timed out\n", GetCurrentThreadId());
                    break; 
                }
        }		
	}
}

void car_enter(void* vptr){
    int car_no = *((int*)vptr); // civilised way of doing it
    printf("Car %d entered \n", car_no);	
}

void car_exit(void* vptr){
    int car_no = *((int*)vptr); // civilised way of doing it
    printf("Car %d exited \n", car_no);	
}