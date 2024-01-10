

/* in C++ :
#include<iostream>
using namespace std;
#include<"object.h">

int main(void)
{
  intQueue queue;

  queue.enqueue(4);
  queue.enqueue(5);

  cout<<queue.dequeue()<<" "<<endl;
  cout<<queue.dequeue()<<" "<<endl;

  queue.enqueue(6);
  queue.enqueue(7);

  cout<<queue.dequeue()<<" "<<endl;
  cout<<queue.dequeue()<<" "<<endl;

  queue.enqueue(42);
  queue.enqueue(43);

  cout<<queue.dequeue()<<" "<<endl;
  cout<<queue.dequeue()<<" "<<endl;

}

*/
#include<pthread.h>
#include<stdio.h>
#include<unistd.h>



#include "ringbuffer.h"

pthread_t sender;
pthread_t receiver;

char* Message[7] = {" this "," is ", " a "," message ", " to "," a "," receiver "};

void* sender_body(void* vptr)
{ 
  //printf(" sender \n");
	
   while(1)
   {  
      printf(" put_mail called: \n");         
      for( unsigned int i = 0; i < 7; i++ )
      {
        sleep(2);
		put_mail(Message[i]);
	  }
	  sleep(2);
	  printf(" try_put_mail called: \n");
	  for( unsigned int i = 0; i < 7; i++ )
      {
        sleep(2);
		while( !try_put_mail(Message[i]) ) sleep(1);
	  }
   }
return NULL;
}

void* receiver_body(void* vptr)
{ char* str;
	//printf(" receiver \n");
   while(1)
   {  
      printf(" get_mail called: \n");   
      for( unsigned int i = 0; i < 7; i++ )
      {
        sleep(1);
		str = (char*)get_mail();
		printf(" %s \n", str );
	  }
      sleep(2);
	  printf(" try_get_mail called: \n");   
	  for( unsigned int i = 0; i < 7; i++ )
      {
        sleep(1);
		str = (char*)try_get_mail();
		if ( str != NULL ) printf(" %s \n", str );
	  }
	  
   }
return NULL;

}

int main(void)
{
	//printf(" main \n");
	initialize_ringbuffer();
	
	
	 pthread_create(&sender, NULL, &sender_body, NULL);
	 pthread_create(&receiver, NULL, &receiver_body, NULL);
	 
	 pthread_join(sender, NULL);
	 pthread_join(receiver, NULL);
	

	return 0;
}

