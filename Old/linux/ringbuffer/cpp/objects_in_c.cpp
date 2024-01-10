// g++ -Wall -Wextra object.cpp objects_in_c.cpp -l pthread
#include<iostream>
#include <pthread.h>
using namespace std;
#include "object.hpp"

pthread_t my_threads[5];

int main(void)
{
  intQueue queue;

  pthread_create(&(my_threads[0]), 
                        NULL, 
                        &queue.enqueue(1) ,
                        NULL);  
  pthread_create(&(my_threads[0]), 
                        NULL, 
                        &queue.enqueue(1) ,
                        NULL);
  //Started writing unit test xD
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
