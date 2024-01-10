#include "ringbuffer.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

//object implementation intQueue begin
// private:
static ELEMENT_TYPE QueueArray[QUEUE_SIZE];
static int queue_start;
static int queue_end;
static int actual_size;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t screenMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;
static pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
// public:
extern void initialize_intQueue(void)
{
	queue_start = 0;
	queue_end = QUEUE_SIZE - 1;
	actual_size = 0;
}

extern void* put_mail(void *a)
{
	pthread_mutex_lock(&mutex);
	while (actual_size == QUEUE_SIZE) pthread_cond_wait(&not_full,&mutex);	
	QueueArray[(++queue_end) % QUEUE_SIZE] = a;
	actual_size++;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&not_empty);

}

extern void* try_put_mail(void *a)
{
	pthread_mutex_lock(&mutex);
	if (actual_size == QUEUE_SIZE) {
		pthread_mutex_unlock(&mutex);
		return 0;
	}
	QueueArray[(++queue_end) % QUEUE_SIZE] = a;
	actual_size++;
	pthread_cond_signal(&not_empty);
	pthread_mutex_unlock(&mutex);
	return 1;
}

extern void* get_mail(void *a)
{
	pthread_mutex_lock(&mutex);
	while (actual_size == 0) pthread_cond_wait(&not_empty,&mutex);
	void* value = QueueArray[(queue_start++) % QUEUE_SIZE];
	actual_size--;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&not_full);
	return value;
}

extern void* try_get_mail(void *a)
{
	pthread_mutex_lock(&mutex);
	if (actual_size == 0) {
		pthread_mutex_unlock(&mutex);
		return NULL;
	}
	void* value = QueueArray[(queue_start++) % QUEUE_SIZE];
	actual_size--;
	pthread_cond_signal(&not_full);
	pthread_mutex_unlock(&mutex);
	return value;
}