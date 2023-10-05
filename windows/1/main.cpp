#include "windows.h"
#include "stdio.h"
#include <iostream>

HANDLE my_threads[5];

enum Problem {a, b, c, d, e };
Problem solution = e;
unsigned char turn = 0;
bool screen_free = true;
int delay = 1000;
HANDLE hMutex;

void print_hi(void);
void print_bye(void);
void thread_1_body(void);
void thread_2_body(void);

int main(void){
    my_threads[0] = CreateThread(
        NULL, // no security attribute
        0,
        (LPTHREAD_START_ROUTINE) thread_1_body, // thread starts 
        NULL, // argument to starting thread 
        0, // start off running 
        NULL// thread ID put here) ;;
    );
    my_threads[1] = CreateThread(
        NULL, // no security attribute
        0,
        (LPTHREAD_START_ROUTINE) thread_2_body, // thread starts 
        NULL, // argument to starting thread 
        0, // start off running 
        NULL // thread ID put here
    );

    if (solution == e) {
            hMutex = CreateMutex(NULL, FALSE, NULL);
            if (hMutex == NULL) //error handling
            {
                printf("CreateMutex error: %d\n", GetLastError());
                return 1;
            }
    }
    WaitForMultipleObjects(2, my_threads, true, INFINITE);

    for (int i = 0; i < 2; i++) 
        CloseHandle(my_threads[i]); 
    CloseHandle(hMutex);
    return 0;
}

void print_hi(void){
    std::cout << "h" << std::flush;
    Sleep(delay);
    std::cout << "i" << std::flush;
    Sleep(delay);
    std::cout << " " << std::flush;
}

void print_bye(void){
    std::cout << "b" << std::flush;
    Sleep(delay);
    std::cout << "y" << std::flush;
    Sleep(delay);
    std::cout << "e" << std::flush;
    Sleep(delay);
    std::cout << " " << std::flush;
}

void thread_1_body(void){
    for (;;){
        switch (solution){
            case a:
            {
                print_hi();
            }
            case b:
            {
                if (turn == 0){
                    print_hi();
                    turn = 1;
                }
            }
            case c:
            {
                if (screen_free == true){
                    screen_free = false;
                    print_hi();
                    screen_free = true;
                }
            }
            case d:
            {
                SuspendThread(my_threads[1]);
                print_hi();
                ResumeThread(my_threads[1]);
            }
            case e:{
                DWORD dwWaitResult;
                dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
                switch (dwWaitResult) 
                {
                    // The thread got ownership of the mutex
                    case WAIT_OBJECT_0: 
                        { 
                            print_hi();   
                        
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
        }
    }
}

void thread_2_body(void){
    for (;;){
        switch (solution){
            case a:
            {
                    print_bye();
            }
            case b:
            {
                if (turn == 1){
                    print_bye();
                    turn = 0;
                }
            }
            case c:
            {
                if (screen_free == true){
                    screen_free = false;
                    print_bye();
                    screen_free = true;
                }
            }
            case d:
            {
                SuspendThread(my_threads[0]);
                print_bye();
                ResumeThread(my_threads[0]);
            }
            case e:{
                DWORD dwWaitResult;
                dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
                switch (dwWaitResult) 
                {
                    // The thread got ownership of the mutex
                    case WAIT_OBJECT_0: 
                        { 
                            print_bye();   
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
        }
    }
}