#include<stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_AMOUNT_OF_CUSTOMERS 5
#define AMOUNT_OF_ENTERING_COSTUMERS 10

pthread_t tBarber, tCustomer[];
pthread_mutex_t mutCustomer = PTHREAD_MUTEX_INITIALIZER;
int number_of_customers = 1;
sem_t semSleep;
sem_t Customer_waiting;

void* barber_func(void* vptr)
{ 
	while(1)
	{  
        sem_wait(&semSleep);
        pthread_mutex_lock(&mutCustomer);
        printf(" barber is cutting hair \n");
        number_of_customers--;
        sem_post(&Customer_waiting);
        pthread_mutex_unlock(&mutCustomer);
        sleep(2);
    }
	return NULL;
}

void* costumer_visits(void* vptr)
{
    sleep(5);
    while(1){
        pthread_mutex_lock(&mutCustomer);
        if (number_of_customers < MAX_AMOUNT_OF_CUSTOMERS){
            sem_post(&semSleep);
            number_of_customers++;
            printf("Customer enters, total: %d \n", number_of_customers);
            pthread_mutex_unlock(&mutCustomer);
        }
        else{
            pthread_mutex_unlock(&mutCustomer);
        }
        sleep(1);
    }
    return NULL;
}


int main(void)
{
    sem_init(&semSleep, 0, 0);
    sem_init(&Customer_waiting, 0, 0);

	pthread_create(&tBarber, NULL, &barber_func, NULL);
    pthread_create(&tCustomer, NULL, &costumer_visits, NULL);
	pthread_join(tBarber, NULL);
    pthread_join(tCustomer, NULL);

	return 0;
}