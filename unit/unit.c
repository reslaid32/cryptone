#include <stdio.h>
#include <stdlib.h>

#include "../queue/aqueue.h"
#include "unit.h"

typedef struct UnitTest
{
  unit_test_fn fn;
  const char  *name;
} UnitTest;

static AtomicQueue m_tst_queue;

__attribute__ ((constructor (103))) static void
_Init ()
{
  AQ_Init (&m_tst_queue);
}

void
UT_Register (unit_test_fn fn, const char *name)
{
  UnitTest *t = malloc (sizeof (UnitTest));
  if (!t)
  {
    fprintf (stderr, "Failed to allocate memory for test registration\n");
    exit (1);
  }
  t->fn   = fn;
  t->name = name;

  AQ_Enqueue (&m_tst_queue, t);
}

int
UT_RunAll (void)
{
  if (AQ_Empty (&m_tst_queue))
  {
    printf ("No tests registered.\n");
    return 1;
  }

  int passed = 0;
  int failed = 0;

  printf ("Running unit tests...\n\n");

  while (!AQ_Empty (&m_tst_queue))
  {
    UnitTest *t = (UnitTest *)AQ_Dequeue (&m_tst_queue);
    if (!t)
      break;

    printf ("[ RUN      ] %s\n", t->name);
    t->fn ();
    printf ("[     OK   ] %s\n\n", t->name);
    free (t);
    passed++;
  }

  AQ_Destroy (&m_tst_queue);

  printf ("Tests done: %d passed, %d failed\n", passed, failed);
  return failed == 0 ? 0 : 1;
}

void
UT_BasicAssert (int cond, const char *cond_str, const char *msg)
{
  if (!cond)
  {
    fprintf (stderr, "Assertion (\"%s\") failed: %s\n", cond_str, msg);
    exit (1);
  }
}
