#ifndef __AQ_IMPL_H
#define __AQ_IMPL_H

#include <pthread.h>

typedef struct AtomicQueue
{
  struct QNode   *front;
  struct QNode   *rear;
  pthread_mutex_t lock;
} AtomicQueue;

void  C1_AQ_Init (AtomicQueue *q);
int   C1_AQ_Empty (AtomicQueue *q);
void  C1_AQ_Enqueue (AtomicQueue *q, void *data);
void *C1_AQ_Dequeue (AtomicQueue *q);
void *C1_AQ_Peek (AtomicQueue *q);
void  C1_AQ_Destroy (AtomicQueue *q);

#endif