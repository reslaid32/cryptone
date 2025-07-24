#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "aqueue.h"
#include "qnode.h"

void
C1_AQ_Init (AtomicQueue *q)
{
  q->front = q->rear = NULL;
  pthread_mutex_init (&q->lock, NULL);
}

int
C1_AQ_Empty (AtomicQueue *q)
{
  pthread_mutex_lock (&q->lock);
  int result = (q->front == NULL);
  pthread_mutex_unlock (&q->lock);
  return result;
}

void
C1_AQ_Enqueue (AtomicQueue *q, void *data)
{
  QNode *newNode = malloc (sizeof (QNode));
  if (!newNode)
  {
    fprintf (stderr, "Memory allocation failed\n");
    exit (EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;

  pthread_mutex_lock (&q->lock);
  if (q->rear == NULL)
  {
    q->front = q->rear = newNode;
  }
  else
  {
    q->rear->next = newNode;
    q->rear       = newNode;
  }
  pthread_mutex_unlock (&q->lock);
}

void *
C1_AQ_Dequeue (AtomicQueue *q)
{
  pthread_mutex_lock (&q->lock);

  if (q->front == NULL)
  {
    pthread_mutex_unlock (&q->lock);
    return NULL;
  }

  QNode *temp = q->front;
  void  *data = temp->data;
  q->front    = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  pthread_mutex_unlock (&q->lock);
  free (temp);
  return data;
}

void *
C1_AQ_Peek (AtomicQueue *q)
{
  pthread_mutex_lock (&q->lock);
  void *data = (q->front != NULL) ? q->front->data : NULL;
  pthread_mutex_unlock (&q->lock);
  return data;
}

void
C1_AQ_Destroy (AtomicQueue *q)
{
  pthread_mutex_lock (&q->lock);
  QNode *node = q->front;
  while (node != NULL)
  {
    QNode *next = node->next;
    free (node);
    node = next;
  }
  q->front = q->rear = NULL;
  pthread_mutex_unlock (&q->lock);
  pthread_mutex_destroy (&q->lock);
}
