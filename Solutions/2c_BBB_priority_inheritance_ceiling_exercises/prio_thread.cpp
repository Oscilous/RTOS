#include<pthread.h>
#include<stdio.h>
#include<unistd.h>

pthread_t A_id;

void* A(void *vptr)
{
	//….
}

int main(void)
{
	struct  sched_param  params;// erase "struct" when programming in C++
	params.sched_priority = 19;
	struct pthread_attr_t   attr;// erase "struct" when programming in C++
	pthread_attr_init(&attr);
	pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setschedparam(&attr, &params);

	pthread_create(&A_id, &attr, &A, NULL);
	pthread_join(A_id, NULL);

	return 0;
}
