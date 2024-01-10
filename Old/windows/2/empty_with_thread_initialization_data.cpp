#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_THREADS 2

HANDLE my_threads[5];

int numbers[2] = { 1,2 };

void my_thread_body(void* vptr)
{
	//int thread_no = (int) vptr; nasty
	int thread_no = *((int*)vptr); // civilised way of doing it
	for (EVER) 
	{
		printf(" thread %d \n", thread_no);	
		Sleep(2000);		
	}
}

int main(void)
{

	for (int i = 0; i < NO_THREADS; i++)
	{
		my_threads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread_body, &numbers[i], 0, NULL);
	}
	
	WaitForMultipleObjects(NO_THREADS, my_threads, true, INFINITE);

	return 0;
}
