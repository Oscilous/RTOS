#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_THREADS 5
#define MAX_SEM_COUNT 3

HANDLE my_threads[NO_THREADS];
// a - mutex, b - semaphore, c - critical section, d - event
enum Problem {a, b, c, d};
Problem solution = a;
HANDLE hMutex, hSemaphore;
int numbers[5] = { 1,2,3,4,5};

void my_thread_body(void* vptr);

int main(void)
{
	switch (solution) {
		case a: {
            hMutex = CreateMutex(NULL, FALSE, NULL);
            if (hMutex == NULL) //error handling
            {
                printf("CreateMutex error: %d\n", GetLastError());
                return 1;
            }
		}
        case b: {
            hSemaphore = CreateSemaphoreW(NULL, (LONG)MAX_SEM_COUNT, (LONG)MAX_SEM_COUNT, NULL);
            if (hSemaphore == NULL) //error handling
            {
                printf("CreateSemaphore error: %d\n", GetLastError());
                return 1;
            }
        }
    }
	for (int i = 0; i < NO_THREADS; i++)
	{
		my_threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_body, &numbers[i], 0, NULL);
	}
	
	WaitForMultipleObjects(NO_THREADS, my_threads, true, INFINITE);
    CloseHandle(hMutex);
    CloseHandle(hSemaphore);
	return 0;
}

void my_thread_body(void* vptr)
{
	//int thread_no = (int) vptr; nasty
	int thread_no = *((int*)vptr); // civilised way of doing it
	for (EVER) 
	{
		switch (solution){
            case a:{
                DWORD dwWaitResult;
                dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
                switch (dwWaitResult) 
                {
                    // The thread got ownership of the mutex
                    case WAIT_OBJECT_0: 
                        { 
                            printf(" thread %d \n", thread_no);	
							Sleep(2000); 
                            // Release ownership of the mutex object
                            if (! ReleaseMutex(hMutex)) 
                            { 
                                // Handle error.
                            } 
                        } 
                        break; 

                    // The thread got ownership of an abandoned mutex
                    case WAIT_ABANDONED: 
                        {
                            // Handle error
                        }
                }
            }
            case b:{}
        }		
	}
}