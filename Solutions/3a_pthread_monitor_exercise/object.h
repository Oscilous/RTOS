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

#define QUEUE_SIZE 4
#define ELEMENT_TYPE int

//object interface intQueue start

// public:
	extern void initialize_monitor(void);
	extern void write_string(char *s);
	extern void write_number(void *arg);
	extern void new_line(void);
	extern void write_string_and_number(char* str, int num);

	
//object intQueue end