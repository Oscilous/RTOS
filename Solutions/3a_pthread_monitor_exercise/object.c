#include "object.h"
#include <stdio.h>
#include <pthread.h>
//object implementation intQueue begin
// private:
static pthread_mutex_t hMutex = PTHREAD_MUTEX_INITIALIZER;

// public:

extern void initialize_monitor(void){
	pthread_mutex_init(&hMutex, NULL);
}

extern void write_string(char *s){
	pthread_mutex_lock(&hMutex);
	printf("%s", s);
	pthread_mutex_unlock(&hMutex);
}

extern void write_number(void *arg){
	int n = *((int*) arg);
	pthread_mutex_lock(&hMutex);
	printf("%d", n);
	pthread_mutex_unlock(&hMutex);
}

extern void new_line(void){
	pthread_mutex_lock(&hMutex);
	printf("\n");
	pthread_mutex_unlock(&hMutex);
}

extern void write_string_and_number(char* str, int num)
{
        pthread_mutex_lock(&hMutex);
        printf("%s %d", str, num);
        pthread_mutex_unlock(&hMutex);
}


//object implementation intQueue end