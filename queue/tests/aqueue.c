#include "../aqueue.h"
#include "../../unit/unit.h"

#include <stddef.h>
#include <stdio.h>

static void
test_init_and_empty (void)
{
  AtomicQueue q;
  C1_AQ_Init (&q);
  UT_Assert (C1_AQ_Empty (&q), "AtomicQueue must be empty after init");
  C1_AQ_Destroy (&q);
}

static void
test_enqueue_dequeue_one (void)
{
  AtomicQueue q;
  C1_AQ_Init (&q);

  int val = 42;
  C1_AQ_Enqueue (&q, &val);
  UT_Assert (!C1_AQ_Empty (&q),
             "AtomicQueue must not be empty after enqueue");

  int *p = (int *)C1_AQ_Dequeue (&q);
  UT_Assert (p != NULL && *p == 42, "Dequeued value must be correct");
  UT_Assert (C1_AQ_Empty (&q), "AtomicQueue must be empty after dequeue");

  C1_AQ_Destroy (&q);
}

static void
test_fifo_order (void)
{
  AtomicQueue q;
  C1_AQ_Init (&q);

  int vals[] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++)
    C1_AQ_Enqueue (&q, &vals[i]);

  for (int i = 0; i < 3; i++)
  {
    int *p = (int *)C1_AQ_Dequeue (&q);
    UT_Assert (p != NULL && *p == vals[i], "FIFO order must be preserved");
  }

  UT_Assert (C1_AQ_Empty (&q),
             "AtomicQueue must be empty after dequeuing all");

  C1_AQ_Destroy (&q);
}

static void
test_peek (void)
{
  AtomicQueue q;
  C1_AQ_Init (&q);

  UT_Assert (C1_AQ_Peek (&q) == NULL,
             "Peek on empty AtomicQueue should return NULL");

  int val = 99;
  C1_AQ_Enqueue (&q, &val);
  int *p = (int *)C1_AQ_Peek (&q);
  UT_Assert (p != NULL && *p == 99,
             "Peek must return first element without dequeue");

  UT_Assert (!C1_AQ_Empty (&q),
             "AtomicQueue must not be empty after peek");

  p = (int *)C1_AQ_Dequeue (&q);
  UT_Assert (p != NULL && *p == 99,
             "Dequeue after peek must return correct element");
  UT_Assert (C1_AQ_Empty (&q), "AtomicQueue must be empty after dequeue");

  C1_AQ_Destroy (&q);
}

static void
test_destroy (void)
{
  AtomicQueue q;
  C1_AQ_Init (&q);

  int vals[] = { 1, 2, 3, 4, 5 };
  for (int i = 0; i < 5; i++)
    C1_AQ_Enqueue (&q, &vals[i]);
  C1_AQ_Destroy (&q);

  UT_Assert (C1_AQ_Empty (&q), "AtomicQueue must be empty after destroy");
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
  UT_Register (_unit, "aqueue/unit");
}
