#include "../aqueue.h"
#include "../../unit/unit.h"

#include <stddef.h>
#include <stdio.h>

static void
test_init_and_empty (void)
{
  AtomicQueue q;
  AQ_Init (&q);
  UT_Assert (AQ_Empty (&q), "AtomicQueue must be empty after init");
  AQ_Destroy (&q);
}

static void
test_enqueue_dequeue_one (void)
{
  AtomicQueue q;
  AQ_Init (&q);

  int val = 42;
  AQ_Enqueue (&q, &val);
  UT_Assert (!AQ_Empty (&q),
             "AtomicQueue must not be empty after enqueue");

  int *p = (int *)AQ_Dequeue (&q);
  UT_Assert (p != NULL && *p == 42, "Dequeued value must be correct");
  UT_Assert (AQ_Empty (&q), "AtomicQueue must be empty after dequeue");

  AQ_Destroy (&q);
}

static void
test_fifo_order (void)
{
  AtomicQueue q;
  AQ_Init (&q);

  int vals[] = { 1, 2, 3 };
  for (int i = 0; i < 3; i++)
    AQ_Enqueue (&q, &vals[i]);

  for (int i = 0; i < 3; i++)
  {
    int *p = (int *)AQ_Dequeue (&q);
    UT_Assert (p != NULL && *p == vals[i], "FIFO order must be preserved");
  }

  UT_Assert (AQ_Empty (&q),
             "AtomicQueue must be empty after dequeuing all");

  AQ_Destroy (&q);
}

static void
test_peek (void)
{
  AtomicQueue q;
  AQ_Init (&q);

  UT_Assert (AQ_Peek (&q) == NULL,
             "Peek on empty AtomicQueue should return NULL");

  int val = 99;
  AQ_Enqueue (&q, &val);
  int *p = (int *)AQ_Peek (&q);
  UT_Assert (p != NULL && *p == 99,
             "Peek must return first element without dequeue");

  UT_Assert (!AQ_Empty (&q), "AtomicQueue must not be empty after peek");

  p = (int *)AQ_Dequeue (&q);
  UT_Assert (p != NULL && *p == 99,
             "Dequeue after peek must return correct element");
  UT_Assert (AQ_Empty (&q), "AtomicQueue must be empty after dequeue");

  AQ_Destroy (&q);
}

static void
test_destroy (void)
{
  AtomicQueue q;
  AQ_Init (&q);

  int vals[] = { 1, 2, 3, 4, 5 };
  for (int i = 0; i < 5; i++)
    AQ_Enqueue (&q, &vals[i]);
  AQ_Destroy (&q);

  UT_Assert (AQ_Empty (&q), "AtomicQueue must be empty after destroy");
}

static void
_unit (void)
{
  __UT_PRINTF ("Running AtomicQueue unit tests...\n");

  test_init_and_empty ();
  test_enqueue_dequeue_one ();
  test_fifo_order ();
  test_peek ();
  test_destroy ();

  __UT_PRINTF ("AtomicQueue unit tests completed.\n");
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "aqueue/unit");
}
