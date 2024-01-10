
/* in C++ :

intQueue::intQueue():queue_start(0), queue_end(queue_size - 1), actual_size(0){}

void intQueue::enqueue(int a)
{
  QueueArray[(++queue_end) % queue_size] = a;
}

int intQueue::dequeue(void)
{
   return QueueArray[(queue_start++) % queue_size];
}

*/
#include "object.h"
//object implementation intQueue begin
// private:
static ELEMENT_TYPE QueueArray[QUEUE_SIZE];
static int queue_start;
static int queue_end;
static int actual_size;
// public:
extern void initialize_intQueue(void)
{
	queue_start = 0; queue_end = QUEUE_SIZE - 1; actual_size = 0;
}

extern void enqueue(int a)
{
	QueueArray[(++queue_end) % QUEUE_SIZE] = a;
}

extern int dequeue(void)
{
	return QueueArray[(queue_start++) % QUEUE_SIZE];
}

//object implementation intQueue end