#ifndef __Q_IMPL_H
#define __Q_IMPL_H

typedef struct Queue
{
  struct QNode *front;
  struct QNode *rear;
} Queue;

void  Q_Init (Queue *q);
int   Q_Empty (Queue *q);
void  Q_Enqueue (Queue *q, void *data);
void *Q_Dequeue (Queue *q);
void *Q_Peek (Queue *q);
void  Q_Destroy (Queue *q);

#endif