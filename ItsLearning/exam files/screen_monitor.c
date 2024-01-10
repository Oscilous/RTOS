#include "screen_monitor.h"

pthread_mutex_t monMutex = PTHREAD_MUTEX_INITIALIZER;

void write_string(char* str)
{
        pthread_mutex_lock(&monMutex);
        printf("%s", str);
        pthread_mutex_unlock(&monMutex);
}

void write_number(int num)
{
        pthread_mutex_lock(&monMutex);
        printf("%d", num);
        pthread_mutex_unlock(&monMutex);
}

void new_line(void)
{
        pthread_mutex_lock(&monMutex);
        printf("\n");
        pthread_mutex_unlock(&monMutex);
}

void write_string_and_number(char* str, int num)
{
        pthread_mutex_lock(&monMutex);
        printf("%s %d", str, num);
        pthread_mutex_unlock(&monMutex);
}
