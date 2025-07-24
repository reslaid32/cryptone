#include "../queue.h"
#include "../../unit/unit.h"

#include <stddef.h>
#include <stdio.h>

static void
test_init_and_empty (void)
{
  Queue q;
  Q_Init (&q);
  UT_Assert (Q_Empty (&q), "Queue must be empty after init");
}

static void
test_enqueue_dequeue_one (void)
{
  Queue q;
  Q_Init (&q);

  int val = 42;
  Q_Enqueue (&q, &val);
  UT_Assert (!Q_Empty (&q), "Queue must not be empty after enqueue");

  int *p = (int *)Q_Dequeue (&q);
  UT_Assert (p != NULL && *p == 42, "Dequeued value must be correct");
  UT_Assert (Q_Empty (&q), "Queue must be empty after dequeue");
}

static void
test_fifo_order (void)
{
  Queue q;
  Q_Init (&q);

  int vals[] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++)
    Q_Enqueue (&q, &vals[i]);

  for (int i = 0; i < 3; i++)
  {
    int *p = (int *)Q_Dequeue (&q);
    UT_Assert (p != NULL && *p == vals[i], "FIFO order must be preserved");
  }

  UT_Assert (Q_Empty (&q), "Queue must be empty after dequeuing all");
}

static void
test_peek (void)
{
  Queue q;
  Q_Init (&q);

  UT_Assert (Q_Peek (&q) == NULL,
             "Peek on empty queue should return NULL");

  int val = 99;
  Q_Enqueue (&q, &val);
  int *p = (int *)Q_Peek (&q);
  UT_Assert (p != NULL && *p == 99,
             "Peek must return the first element without dequeue");

  UT_Assert (!Q_Empty (&q), "Queue must not be empty after peek");

  p = (int *)Q_Dequeue (&q);
  UT_Assert (p != NULL && *p == 99,
             "Dequeue after peek must return correct element");
  UT_Assert (Q_Empty (&q), "Queue must be empty after dequeue");
}

static void
test_destroy (void)
{
  Queue q;
  Q_Init (&q);

  int vals[] = { 1, 2, 3, 4, 5 };
  for (int i = 0; i < 5; i++)
    Q_Enqueue (&q, &vals[i]);
  Q_Destroy (&q);

  UT_Assert (Q_Empty (&q), "Queue must be empty after destroy");
}

static void
_unit (void)
{
  test_init_and_empty ();
  test_enqueue_dequeue_one ();
  test_fifo_order ();
  test_peek ();
  test_destroy ();
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "queue/unit");
}
