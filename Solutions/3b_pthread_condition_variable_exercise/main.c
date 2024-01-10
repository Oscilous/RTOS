// gcc -Wall -Wextra ringbuffer.c main.c -l pthread

#include<stdio.h>
#include "ringbuffer.h"
#include <pthread.h>
#include <unistd.h>

pthread_t sender;
pthread_t receiver;
pthread_t printer;

char* Message[7] = {" this "," is ", " a "," message ", " to "," a "," receiver "};

void* sender_body(void* vptr)
{ 
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
			while( !try_put_mail(Message[i]) )
				sleep(1);
		}
	}
	return NULL;
}

void* receiver_body(void* vptr)
{ 
	char* str;
	while(1)
	{  
		printf(" get_mail called: \n");   
		for( unsigned int i = 0; i < 7; i++ ){
			sleep(1);
			str = (char*)get_mail(NULL);
			printf(" %s \n", str );
		}
		sleep(2);
		printf(" try_get_mail called: \n");   
		for( unsigned int i = 0; i < 7; i++ ){
			sleep(1);
			str = (char*)try_get_mail(NULL);
			if ( str != NULL ) 
				printf(" %s \n", str );
		}
	}
	return NULL;
}


void* sender_body_a(void* vptr)
{ 
	while(1)
	{  
		printf(" put_mail called: \n");         
		for( unsigned int i = 0; i < 7; i++ )
		{
			sleep(2);
			put_mail(Message[i]);
		}
		sleep(2);
	}
	return NULL;
}

void* receiver_body_a(void* vptr)
{ 
	char* str;
	while(1)
	{  
		printf(" get_mail called: \n");   
		for( unsigned int i = 0; i < 7; i++ ){
			sleep(1);
			str = (char*)get_mail(NULL);
			printf(" %s \n", str );
		}
		sleep(2);
	}
	return NULL;
}



int main(void)
{
	initialize_intQueue();

	pthread_create(&sender, NULL, &sender_body_a, &Message);
	pthread_create(&receiver, NULL, &receiver_body_a, NULL);
	//pthread_create(&printer, NULL, print_buffer, NULL);

	pthread_join(sender, NULL);
	pthread_join(receiver, NULL);
	//pthread_join(printer, NULL);

	return 0;
}