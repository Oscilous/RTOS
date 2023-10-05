#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_THREADS 5
#define MAX_SEM_COUNT 3

HANDLE my_threads[NO_THREADS];
// a - mutex, b - semaphore, c - critical section, d - event
enum Problem {a, b, c, d};
Problem solution = d;
HANDLE hMutex, hSemaphore, hEvent;
CRITICAL_SECTION CriticalSection;
int numbers[5] = {1,2,3,4,5};

void my_thread_body(void* vptr);

int main(void)
{
    for (int i = 0; i < NO_THREADS; i++)
	{
		my_threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_body, &numbers[i], 0, NULL);
	}

	switch (solution) {
		case a: {
            hMutex = CreateMutex(NULL, FALSE, NULL);
            if (hMutex == NULL) //error handling
            {
                printf("CreateMutex error: %d\n", GetLastError());
                return 1;
            }
            WaitForMultipleObjects(NO_THREADS, my_threads, true, INFINITE);
            break;
		}
        case b: {
            hSemaphore = CreateSemaphoreW(NULL, (LONG)MAX_SEM_COUNT, (LONG)MAX_SEM_COUNT, NULL);
            if (hSemaphore == NULL) //error handling
            {
                printf("CreateSemaphore error: %d\n", GetLastError());
                return 1;
            }
            WaitForMultipleObjects(NO_THREADS, my_threads, true, INFINITE);
            break;
        }
        case c:{
            if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400) ) {
                printf("InitializeCriticalSectionAndSpinCount error: %d\n", GetLastError());
                return 1;
            }
            while(1);
            break;
        }
        case d:{
            hEvent = CreateEvent( 
                NULL,               // default security attributes
                TRUE,               // manual-reset event
                FALSE,              // initial state is nonsignaled
                TEXT("Event")  // object name
                ); 

            if (hEvent == NULL) 
            { 
                printf("CreateEvent failed (%d)\n", GetLastError());
                return 1;
            }

            if (! SetEvent(hEvent) ) 
            {
                printf("SetEvent failed (%d)\n", GetLastError());
                return 1;
            }
            WaitForMultipleObjects(NO_THREADS, my_threads, true, INFINITE);
            break;
        }
    }

    for(int i=0; i < NO_THREADS; i++)
        CloseHandle(my_threads[i]);
    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
    CloseHandle(hEvent);
    DeleteCriticalSection(&CriticalSection);
	return 0;
}

void my_thread_body(void* vptr)
{
	//int thread_no = (int) vptr; nasty
	int thread_no = *((int*)vptr); // civilised way of doing it
	for (EVER) 
	{
        DWORD dwWaitResult; 
		switch (solution){
            case a:{
                dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
                switch (dwWaitResult) 
                {
                    // The thread got ownership of the mutex
                    case WAIT_OBJECT_0: 
                    { 
                        printf(" thread %d \n", thread_no);	
                        Sleep(2000); 
                        // Release ownership of the mutex object
                        if (! ReleaseMutex(hMutex)) { 
                            // Handle error.
                        } 
                        break;
                    } 
                    // The thread got ownership of an abandoned mutex
                    case WAIT_ABANDONED: {
                        // Handle error
                        break;
                    }
                }
                break;
            }
            case b:{
                // Try to enter the semaphore gate.
                dwWaitResult = WaitForSingleObject( 
                    hSemaphore,   // handle to semaphore
                    0L);           // zero-second time-out interval

                switch (dwWaitResult) 
                { 
                    // The semaphore object was signaled.
                    case WAIT_OBJECT_0: 
                    {
                        printf(" thread %d \n", thread_no);	
                        Sleep(2000);

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
                break;
            }
            case c:{
                // Request ownership of the critical section.
                EnterCriticalSection(&CriticalSection); 

                printf(" thread %d \n", thread_no);	
                Sleep(2000); 
                
                // Release ownership of the critical section.
                LeaveCriticalSection(&CriticalSection);
                break;
            }
            case d:{
                dwWaitResult = WaitForSingleObject( 
                    hEvent, // event handle
                    INFINITE);    // indefinite wait
                switch (dwWaitResult) 
                {
                    // Event object was signaled
                    case WAIT_OBJECT_0: 
                    {
                        printf(" thread %d \n", thread_no);	
                        Sleep(2000);
                        break; 
                    }
                    // An error occurred
                    default: 
                    {
                        printf("Wait error (%d)\n", GetLastError());
                        break; 
                    }
                }
                break;
            }
        }		
	}
}