#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

#define EVER ;; 
#define NO_THREADS 5

HANDLE my_threads[NO_THREADS];

int add_number_1, add_number_2, add_result;
int sub_number_1, sub_number_2, sub_result;
int mul_number_1, mul_number_2, mul_result;
int div_number_1, div_number_2, div_result;

HANDLE mul_operands_ready, mul_result_ready ;
HANDLE div_operands_ready, div_result_ready ;
HANDLE add_operands_ready, add_result_ready ;
HANDLE sub_operands_ready, sub_result_ready ;

void addition(void)
{
	for (EVER) 
	{
        WaitForSingleObject( 
                    add_operands_ready, // event handle
                    INFINITE);    // indefinite wait
        add_result = add_number_1 + add_number_2;
        ResetEvent( mul_operands_ready);
        SetEvent(add_result_ready);
    }
}

void subtraction(void)
{
	for (EVER) 
	{
        WaitForSingleObject( 
                    sub_operands_ready, // event handle
                    INFINITE);    // indefinite wait
        sub_result = sub_number_1 - sub_number_2;
        ResetEvent( sub_operands_ready);
        SetEvent(sub_result_ready);
    }
}

void multiply(void)
{
	for (EVER) 
	{
        WaitForSingleObject( 
                    mul_operands_ready, // event handle
                    INFINITE);    // indefinite wait
        mul_result = mul_number_1 * mul_number_2;
        ResetEvent( mul_operands_ready);
        SetEvent(mul_result_ready);
    }
}

void divide(void)
{
	for (EVER) 
	{
        WaitForSingleObject( 
                    div_operands_ready, // event handle
                    INFINITE);    // indefinite wait
        div_result = div_number_1 / div_number_2;
        ResetEvent( div_operands_ready);
        SetEvent(div_result_ready);
    }
}


void calculation(void){
    for (EVER) 
	{
        mul_number_1 = 4;
        mul_number_2 = 3;
        SetEvent(mul_operands_ready);

        div_number_1 = 8;
        div_number_2 = 2;
        SetEvent(div_operands_ready);

        HANDLE parralel[2];
        parralel[0] = mul_result_ready;
        parralel[1] = div_result_ready;

        WaitForMultipleObjects(
                    2, // number of objects in array
                    parralel, // event handle
                    1, // wait for all objects
                    INFINITE);    // indefinite wait
        ResetEvent( mul_result_ready);
        ResetEvent( div_result_ready);
        printf("%d    %d    %d    %d \n", add_result, sub_result, mul_result, div_result);


        sub_number_1 = mul_result;
        sub_number_2 = 2;
        SetEvent(sub_operands_ready);
        WaitForSingleObject( 
                    sub_result_ready, // event handle
                    INFINITE);    // indefinite wait
        ResetEvent( sub_result_ready);
        printf("%d    %d    %d    %d \n", add_result, sub_result, mul_result, div_result);

        add_number_1 = sub_result;
        add_number_2 = div_result;
        SetEvent(add_operands_ready);
        WaitForSingleObject( 
                    add_result_ready, // event handle
                    INFINITE);    // indefinite wait
        ResetEvent( add_result_ready);
        printf("%d    %d    %d    %d \n", add_result, sub_result, mul_result, div_result);

        printf("Answer: %d\n", add_result);
        Sleep(1000);
    }
}

int main(void)
{
    printf("+    -    *    / \n");
    my_threads[0] = CreateThread(NULL, // default security
                                0, // default stack size
                                (LPTHREAD_START_ROUTINE)addition, // name of the thread function
                                NULL,  // no thread parameters
                                0, // default startup flags
                                NULL); // receive thread identifier
    my_threads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)subtraction, NULL, 0, NULL);
    my_threads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)multiply, NULL, 0, NULL);
    my_threads[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)divide, NULL, 0, NULL);
    my_threads[4] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)calculation, NULL, 0, NULL);


    add_operands_ready = CreateEvent( 
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        NULL  // object name
        ); 
    add_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

    sub_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
    sub_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

    mul_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
    mul_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);

    div_operands_ready = CreateEvent(NULL,TRUE,FALSE,NULL);
    div_result_ready   = CreateEvent(NULL,TRUE,FALSE,NULL);  


	WaitForMultipleObjects(NO_THREADS, my_threads, 1, INFINITE);
    for(int i=0; i < NO_THREADS; i++)
        CloseHandle(my_threads[i]);
    CloseHandle(add_operands_ready);
    CloseHandle(sub_operands_ready);
    CloseHandle(mul_operands_ready);
    CloseHandle(div_operands_ready);
    CloseHandle(add_result_ready);
    CloseHandle(sub_result_ready);
    CloseHandle(mul_result_ready);
    CloseHandle(div_result_ready);

	return 0;
}