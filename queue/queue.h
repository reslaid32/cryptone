#ifndef __Q_IMPL_H
#define __Q_IMPL_H

typedef struct Queue
{
  struct QNode *front;
  struct QNode *rear;
} Queue;

void  C1_Q_Init (Queue *q);
int   C1_Q_Empty (Queue *q);
void  C1_Q_Enqueue (Queue *q, void *data);
void *C1_Q_Dequeue (Queue *q);
void *C1_Q_Peek (Queue *q);
void  C1_Q_Destroy (Queue *q);

#endif