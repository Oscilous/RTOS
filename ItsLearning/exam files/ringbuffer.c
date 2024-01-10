#include "ringbuffer.h"

static void* QueueArray[QUEUE_SIZE];
static int queue_start;
static int queue_end;
static int actual_size;

static pthread_mutex_t ringbuffer_mutex;
static pthread_cond_t full_cond, empty_cond;

extern void initialize_ringbuffer(void)
{
	pthread_mutex_init(&ringbuffer_mutex,NULL);
	pthread_cond_init(&full_cond,NULL);
	pthread_cond_init(&empty_cond,NULL);
	queue_start = 0; queue_end = QUEUE_SIZE - 1; actual_size = 0;
}

extern void put_mail(void* a)
{
   pthread_mutex_lock(&ringbuffer_mutex); 
        while (actual_size == QUEUE_SIZE) pthread_cond_wait(&full_cond,&ringbuffer_mutex); 
        QueueArray[(++queue_end) % QUEUE_SIZE] = a;
	    actual_size++;
        pthread_cond_signal(&empty_cond);
   pthread_mutex_unlock(&ringbuffer_mutex);
}

extern int try_put_mail(void* a)
{
   pthread_mutex_lock(&ringbuffer_mutex); 
        if (actual_size == QUEUE_SIZE) 
		{
			pthread_mutex_unlock(&ringbuffer_mutex);
			return 0;
		}
        QueueArray[(++queue_end) % QUEUE_SIZE] = a;
	    actual_size++;
        pthread_cond_signal(&empty_cond);
   pthread_mutex_unlock(&ringbuffer_mutex);
   return 1;
}

extern void* get_mail(void)
{
   pthread_mutex_lock(&ringbuffer_mutex); 
       while (actual_size == 0) pthread_cond_wait(&empty_cond,&ringbuffer_mutex);	
       actual_size--;
       void* temp = QueueArray[(queue_start++) % QUEUE_SIZE];
       pthread_cond_signal(&full_cond);
   pthread_mutex_unlock(&ringbuffer_mutex);
   return temp;
}

extern void* try_get_mail(void)
{
   pthread_mutex_lock(&ringbuffer_mutex); 
       if (actual_size == 0) 
	   {
			pthread_mutex_unlock(&ringbuffer_mutex);
			return NULL;
		}
       actual_size--;
       void* temp = QueueArray[(queue_start++) % QUEUE_SIZE];
       pthread_cond_signal(&full_cond);
   pthread_mutex_unlock(&ringbuffer_mutex);
   return temp;
}

