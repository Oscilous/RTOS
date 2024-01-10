#ifndef _OBJECT_H_
#define _OBJECT_H_
#include <pthread.h>

const int queue_size = 4;

class intQueue
{
 public:
	intQueue();

	void enqueue(int a);

	int dequeue(void);
    
 private:
	pthread_mutex_t mutex;
	pthread_cond_t empty_cond;
	pthread_cond_t full_cond;
	unsigned int is_empty;
	unsigned int is_full;
	int QueueArray[queue_size];
	int queue_start, queue_end, actual_size;
	void update_bool();
};
#endif