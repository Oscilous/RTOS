#include "object.hpp"
#include <pthread.h>

intQueue::intQueue():queue_start(0), queue_end(queue_size - 1), actual_size(0){
  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&empty_cond, NULL);
  pthread_cond_init(&full_cond, NULL);
  is_empty = true;
  is_full = false;
}

void intQueue::enqueue(int a)
{
  pthread_mutex_lock(&mutex);
  actual_size++;
  update_bool();
  pthread_cond_signal(&empty_cond);
  while (is_full)
    pthread_cond_wait(&full_cond, &mutex);
  QueueArray[(++queue_end) % queue_size] = a;
  pthread_mutex_unlock(&mutex);
}

int intQueue::dequeue(void)
{
  pthread_mutex_lock(&mutex);
  actual_size--;
  update_bool();
  while (is_empty)
    pthread_cond_wait(&empty_cond, &mutex);
  int index = (queue_start++) % queue_size;
  pthread_mutex_unlock(&mutex);
  return QueueArray[index];
}

void intQueue::update_bool(void){
  is_full = actual_size / 4;
  is_empty = actual_size;
}