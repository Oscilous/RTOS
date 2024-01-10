#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_THREADS 10
#define MAX_SEM_COUNT 3

HANDLE my_threads[NO_THREADS];
HANDLE hParkingSemaphore, hRoadOneMutex, hRoadTwoMutex, hScreenMutex;

int numbers[10] = {1,2,3,4,5,6,7,8,9,10};
int initial_flag = 0;

void print_status(int thread_no, char letter){
    WaitForSingleObject(hScreenMutex, INFINITE);
    for (int spacing = 1; spacing < thread_no; spacing++)
    {
        printf("     ");
    }
    printf("%c\n", letter);
    ReleaseMutex(hScreenMutex);
}


void my_thread_body(void* vptr)
{
	//int thread_no = (int) vptr; nasty
	int thread_no = *((int*)vptr); // civilised way of doing it
	for (EVER) 
	{
        WaitForSingleObject(hParkingSemaphore, INFINITE);
        print_status(thread_no, 'w');
        if (WaitForSingleObject(hRoadOneMutex, 0) == WAIT_TIMEOUT){
            WaitForSingleObject(hRoadTwoMutex, INFINITE);
            print_status(thread_no, 't');
            Sleep(2000);
            ReleaseMutex(hRoadTwoMutex);
        }
        else{
            print_status(thread_no, 'o');	
            Sleep(2000); 	
            ReleaseMutex(hRoadOneMutex);
        }
        print_status(thread_no, 'p');	
        Sleep(5000);
        if (WaitForSingleObject(hRoadOneMutex, 0) == WAIT_TIMEOUT){
            WaitForSingleObject(hRoadTwoMutex, INFINITE);
            print_status(thread_no, 't');
            ReleaseSemaphore(hParkingSemaphore, 1, NULL);
            Sleep(2000);
            ReleaseMutex(hRoadTwoMutex);
        }
        else{
            print_status(thread_no, 'o');
            ReleaseSemaphore(hParkingSemaphore, 1, NULL);	
            Sleep(2000); 	
            ReleaseMutex(hRoadOneMutex);
        }
        print_status(thread_no, 'l');
	}
}

int main(void)
{
    printf("w - waiting, o - road one, t - road two, p - parking, l - left\n");
    for (int i = 0; i < NO_THREADS; i++)
	{
        printf("%d    ", numbers[i]);
	}
    printf("\n");
	for (int i = 0; i < NO_THREADS; i++)
	{
		my_threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_body, &numbers[i], 0, NULL);
	}

    hParkingSemaphore = CreateSemaphoreW(NULL, (LONG)MAX_SEM_COUNT, (LONG)MAX_SEM_COUNT, NULL);
    hRoadOneMutex = CreateMutex(NULL, FALSE, NULL);
    hRoadTwoMutex = CreateMutex(NULL, FALSE, NULL);
    hScreenMutex = CreateMutex(NULL, FALSE, NULL);

	WaitForMultipleObjects(NO_THREADS, my_threads, 1, INFINITE);
    for(int i=0; i < NO_THREADS; i++)
        CloseHandle(my_threads[i]);
    CloseHandle(hParkingSemaphore);
    CloseHandle(hRoadOneMutex);

	return 0;
}