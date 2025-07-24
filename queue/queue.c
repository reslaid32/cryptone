#include <stdio.h>
#include <stdlib.h>

#include "qnode.h"
#include "queue.h"

void
C1_Q_Init (Queue *q)
{
  q->front = q->rear = NULL;
}

int
C1_Q_Empty (Queue *q)
{
  return q->front == NULL;
}

void
C1_Q_Enqueue (Queue *q, void *data)
{
  QNode *newNode = malloc (sizeof (QNode));
  if (!newNode)
  {
    fprintf (stderr, "Memory allocation failed\n");
    exit (EXIT_FAILURE);
  }
  newNode->data = data;
  newNode->next = NULL;

  if (q->rear == NULL)
  {
    q->front = q->rear = newNode;
  }
  else
  {
    q->rear->next = newNode;
    q->rear       = newNode;
  }
}

void *
C1_Q_Dequeue (Queue *q)
{
  if (C1_Q_Empty (q))
    return NULL;

  QNode *temp = q->front;
  void  *data = temp->data;
  q->front    = q->front->next;

  if (q->front == NULL)
    q->rear = NULL;

  free (temp);
  return data;
}

void *
C1_Q_Peek (Queue *q)
{
  if (C1_Q_Empty (q))
    return NULL;
  return q->front->data;
}

void
C1_Q_Destroy (Queue *q)
{
  while (!C1_Q_Empty (q))
    C1_Q_Dequeue (q);
}