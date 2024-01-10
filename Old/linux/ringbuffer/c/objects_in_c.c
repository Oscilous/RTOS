

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

#include<stdio.h>

#include "object.h"

int main(void)
{
	initialize_intQueue();
	
  //queue_start = 0; // not allowed queuestart is private ( only accessible in object.c )

	enqueue(4);
	enqueue(5);

	printf(" %d \n", dequeue()); 
	printf(" %d \n", dequeue());

	enqueue(6);
	enqueue(7);

	printf(" %d \n", dequeue());
	printf(" %d \n", dequeue());

	enqueue(42);
    enqueue(43);

	printf(" %d \n", dequeue());
	printf(" %d \n", dequeue());

	return 0;
}