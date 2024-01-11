// sudo taskset -c 2 ./main
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

#define NUM_THREADS 2

void *prime(void *arg){
    int i, n = 2;
    while (n < 1000000000) { 
        for (i = 2; i <= n; i++){
            if (n % i == 0) 
                break;
        }
        if (i == n) {
            printf(" %d", n);
            fflush(stdout);
        }
        n++;
        //usleep(1000);
    }
    return NULL;
}

void *clock_function(void *arg){
    long int seconds = 0;
    long int decisecond = 0;
    int minutes = 0;
    while(1){
        while (seconds != 60){
            while (decisecond != 10){
                usleep(100000);
                decisecond++;
                printf(" \n \n Minutes: %d;Seconds: %ld; Deciseconds: %ld \n \n", minutes, seconds, decisecond);
                fflush(stdout);
            }
            decisecond = 0;
            seconds++;
        }
        seconds = 0;
        minutes++;
    }
}

int main(void){
    pthread_t threads[NUM_THREADS];

    pthread_attr_t prime_attr;
    pthread_attr_init(&prime_attr);
    pthread_attr_setinheritsched(&prime_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&prime_attr, SCHED_FIFO);
    struct sched_param prime_params;
    prime_params.sched_priority = 1;
    pthread_attr_setschedparam(&prime_attr, &prime_params);

    pthread_attr_t clock_attr;
    pthread_attr_init(&clock_attr);
    pthread_attr_setinheritsched(&clock_attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&clock_attr, SCHED_FIFO);
    struct sched_param clock_params;
    clock_params.sched_priority = 2;
    pthread_attr_setschedparam(&clock_attr, &clock_params);

    pthread_create(&threads[0], &prime_attr, prime, NULL);
    pthread_create(&threads[1], &clock_attr, clock_function, NULL);
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    return 0;
}
