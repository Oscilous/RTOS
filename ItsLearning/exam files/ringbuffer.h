/* in C++ :
const int queue_size = 4;

class intQueue
{
 public:
	intQueue();

	void enqueue(int a);

	int dequeue(void);
    
 private:
	int QueueArray[queue_size];
	int queue_start, queue_end, actual_size;
};
*/
#include<pthread.h>
#define QUEUE_SIZE 4


//object interface intQueue start

// public:
	extern void initialize_ringbuffer(void);

	extern void put_mail(void* a);

	extern void* get_mail(void);
	
	extern int try_put_mail(void* a);

	extern void* try_get_mail(void);
	
//object intQueue end
