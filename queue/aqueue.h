#ifndef __AQ_IMPL_H
#define __AQ_IMPL_H

#include <pthread.h>

typedef struct AtomicQueue
{
  struct QNode   *front;
  struct QNode   *rear;
  pthread_mutex_t lock;
} AtomicQueue;

void  AQ_Init (AtomicQueue *q);
int   AQ_Empty (AtomicQueue *q);
void  AQ_Enqueue (AtomicQueue *q, void *data);
void *AQ_Dequeue (AtomicQueue *q);
void *AQ_Peek (AtomicQueue *q);
void  AQ_Destroy (AtomicQueue *q);

#endif