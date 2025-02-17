/* 
 * Copyright (C) 2009 Chris Simmonds (chris@2net.co.uk)
 *
 * This is a demonstration of periodic threads using the Linux timerfd
 * interface which was introduced in GNU libc 2.8 and kernel 2.6.25.
 */

// sudo taskset -c 2 ./main

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#include <time.h>
#include <iostream>
#include <fstream>  
using namespace std;

// 89000000 is 1 sec:
#define N_CPU_BURN 590000000


pthread_t t_0;
pthread_t t_1;
pthread_t t_2;
pthread_t t_3;

pthread_mutex_t Mutex;

struct periodic_info {
	int timer_fd;
	unsigned long long wakeups_missed;
};


static int make_periodic(unsigned int period, struct periodic_info *info)
{
	int ret;
	unsigned int ns;
	unsigned int sec;
	int fd;
	struct itimerspec itval;

	/* Create the timer */
	fd = timerfd_create(CLOCK_MONOTONIC, 0);
	info->wakeups_missed = 0;
	info->timer_fd = fd;
	if (fd == -1)
		return fd;

	/* Make the timer periodic */
	sec = period / 1000000;
	ns = (period - (sec * 1000000)) * 1000;
	itval.it_interval.tv_sec = sec;
	itval.it_interval.tv_nsec = ns;
	itval.it_value.tv_sec = sec;
	itval.it_value.tv_nsec = ns;
	ret = timerfd_settime(fd, 0, &itval, NULL);
	return ret;
}


static void wait_period(struct periodic_info *info)
{
	unsigned long long missed;
	int ret;

	/* Wait for the next timer event. If we have missed any the
	   number is written to "missed" */
	ret = read(info->timer_fd, &missed, sizeof(missed));
	if (ret == -1) {
		perror("read timer");
		return;
	}

	info->wakeups_missed += missed;
}


static void CPUburn(unsigned long n)
{
	for (unsigned long i = 0; i<= n;i++) 
	{
	  // do nothing  
	}
}

void work(int time_units, int task_no, char output) 
{ 
  static unsigned int globalTime = 0;
  for(int j = 0; j < time_units; j++)
  {
		printf("%d:",globalTime);
		globalTime++;
		for(int j = 0; j < task_no ; j++) printf("\t");
		printf("%d%c\n",task_no,output);
		fflush(stdout);
		CPUburn(N_CPU_BURN);
  }
}

static void* idle_thread(void *vptr)
{
 int thread_no = *(int*) vptr;	
 while(1)
 {
   work(1,thread_no,' ');
 }
 return NULL;
}

static void *thread_1(void *vptr)
{
	
        int thread_no = *(int*) vptr;	
	struct periodic_info info;

	//printf("Thread 1 period 20 s\n");
	make_periodic(20000000, &info);
	while (1) 
	{
		// thread work here (call work function and perhaps use the mutex) 
		wait_period(&info);
	}
	return NULL;
}

static void *thread_2(void *vptr)
{

        int thread_no = *(int*) vptr;	
	struct periodic_info info;

	//printf("Thread 2 period 20 s\n");
	make_periodic(20000000, &info);
	while (1) {
		// thread work here (call work function and perhaps use the mutex)  		
		wait_period(&info);
	}
	return NULL;
}

static void *thread_3(void *vptr)
{

        int thread_no = *(int*) vptr;	
	struct periodic_info info;

	//printf("Thread 3 period 20 s\n");
	make_periodic(20000000, &info);
	while (1) {
		        
		// thread work here (call work function and perhaps use the mutex) 	
		wait_period(&info);
	}
	return NULL;
}



int main(int argc, char *argv[])
{

	// work calibration of N_CPU_BURN to 1 sec.:
	//int s,e;s=clock();work(1,0,'C');e=clock();cout<<endl<<e-s<<"us"<<endl;	
	
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
    //pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT);
	 pthread_mutex_init(&Mutex, &attr);

	// Scheduling parameters
	
	struct sched_param params_t0;
	params_t0.sched_priority = 19;

	struct sched_param params_t1;
	params_t1.sched_priority = 23;

	struct sched_param params_t2;
	params_t2.sched_priority = 24;
	
	struct sched_param params_t3;
	params_t3.sched_priority = 25;
	
	
        pthread_attr_t attr_t0;
	pthread_attr_t attr_t1;
        pthread_attr_t attr_t2;
	pthread_attr_t attr_t3;
		
	pthread_attr_init(&attr_t0);
	pthread_attr_init(&attr_t1);
	pthread_attr_init(&attr_t2);
		pthread_attr_init(&attr_t3);


	pthread_attr_setinheritsched(&attr_t0, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t1, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t2, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setinheritsched(&attr_t3, PTHREAD_EXPLICIT_SCHED);
	


	pthread_attr_setschedpolicy(&attr_t0, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t1, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t2, SCHED_FIFO);
	pthread_attr_setschedpolicy(&attr_t3, SCHED_FIFO);



	pthread_attr_setschedparam(&attr_t0, &params_t0);
	pthread_attr_setschedparam(&attr_t1, &params_t1);
	pthread_attr_setschedparam(&attr_t2, &params_t2);
	pthread_attr_setschedparam(&attr_t3, &params_t3);	

	
	pthread_create(&t_3, &attr_t3, thread_3, (void*)3 );
	pthread_create(&t_2, &attr_t2, thread_2, (void*)2 );
	pthread_create(&t_1, &attr_t1, thread_1, (void*)1 );
    pthread_create(&t_0, &attr_t0, idle_thread, (void*)0 );

	pthread_join(t_0,NULL);
	pthread_join(t_1,NULL);
	pthread_join(t_2,NULL);
	pthread_join(t_3,NULL);

	sleep(200);
	return 0;
}
