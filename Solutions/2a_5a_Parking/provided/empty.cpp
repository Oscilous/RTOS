#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 

HANDLE my_threads[5];

void my_thread1_body(void)
{
	for (EVER)
	{
		printf("  thread 1 \n");
		Sleep(1000);	
	}
}

void my_thread2_body(void)
{
	for (EVER)
	{
		printf(" thread 2 \n");	
		Sleep(2000);		
	}
}

int main(void)
{
	
	my_threads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread1_body, NULL, 0, NULL);
	my_threads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)my_thread2_body, NULL, 0, NULL);
	
	WaitForMultipleObjects(2, my_threads, true, INFINITE);

	return 0;
}
