#include<pthread.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include <sched.h>
using namespace std;

pthread_t primes_id;

void* primes(void *vptr)
{
	unsigned int i, n = 2;
	while (n < 1000000000)
	{
		for (i = 2; i <= n; i++) if (n % i == 0) break;
		if (i == n) cout << " " << n << flush;
		n++;
	}
        return NULL;

}

pthread_t seconds_clock_id;

void* seconds_clock(void *vptr)
{ 
  int s=0;
  while(1)
  { 
    cout << endl << s << endl << flush;
    sleep(1);
    s++;
  }
  return NULL;
}

int main(void)
{   
  cout << "hello world" << flush;
   
  sched_param primes_params;
  primes_params.sched_priority = 19;
  pthread_attr_t primes_attr;
  pthread_attr_init(&primes_attr);
  pthread_attr_setinheritsched(&primes_attr,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&primes_attr,SCHED_FIFO);
  pthread_attr_setschedparam(&primes_attr,&primes_params);
  pthread_create(&primes_id, &primes_attr, &primes, NULL);
 
  sched_param seconds_clock_params;
  seconds_clock_params.sched_priority = 20;
  pthread_attr_t seconds_clock_attr;
  pthread_attr_init(&seconds_clock_attr);
  pthread_attr_setinheritsched(&seconds_clock_attr,PTHREAD_EXPLICIT_SCHED);
  pthread_attr_setschedpolicy(&seconds_clock_attr,SCHED_FIFO);
  pthread_attr_setschedparam(&seconds_clock_attr,&seconds_clock_params);
  pthread_create(&seconds_clock_id, &seconds_clock_attr, &seconds_clock, NULL);

  pthread_join(primes_id,NULL);
  pthread_join(seconds_clock_id,NULL);
  cout << "bye cruel world";
 return 0;
}


