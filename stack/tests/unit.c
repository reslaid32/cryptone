#include "../../unit/unit.h"
#include "../stack.h"

static void
test_push_pop_ints (void)
{
  Stack s;
  C1_Stack_Init (&s, sizeof (int), 5);

  int a = 10, b = 20, c = 30;
  UT_Assert (C1_Stack_Push (&s, &a), "push a failed");
  UT_Assert (C1_Stack_Push (&s, &b), "push b failed");
  UT_Assert (C1_Stack_Push (&s, &c), "push c failed");
  UT_Assert (C1_Stack_Size (&s) == 3, "size should be 3");

  int out = 0;
  UT_Assert (C1_Stack_Peek (&s, &out), "peek failed");
  UT_Assert (out == 30, "peeked value should be 30");

  UT_Assert (C1_Stack_Pop (&s, &out), "pop c failed");
  UT_Assert (out == 30, "expected 30");

  UT_Assert (C1_Stack_Pop (&s, &out), "pop b failed");
  UT_Assert (out == 20, "expected 20");

  UT_Assert (C1_Stack_Pop (&s, &out), "pop a failed");
  UT_Assert (out == 10, "expected 10");

  UT_Assert (C1_Stack_Empty (&s), "stack should be empty");

  C1_Stack_Destroy (&s);
}

static void
test_push_struct (void)
{
  typedef struct
  {
    int   id;
    float score;
  } Item;

  Stack s;
  C1_Stack_Init (&s, sizeof (Item), 3);

  Item i1 = { 1, 99.5f };
  Item i2 = { 2, 88.0f };
  UT_Assert (C1_Stack_Push (&s, &i1), "push i1 failed");
  UT_Assert (C1_Stack_Push (&s, &i2), "push i2 failed");

  Item out = { 0 };
  UT_Assert (C1_Stack_Peek (&s, &out), "peek failed");
  UT_Assert (out.id == 2, "peek id should be 2");
  UT_Assert (out.score == 88.0f, "peek score should be 88.0");

  UT_Assert (C1_Stack_Pop (&s, &out), "pop i2 failed");
  UT_Assert (out.id == 2 && out.score == 88.0f, "popped i2 mismatch");

  UT_Assert (C1_Stack_Pop (&s, &out), "pop i1 failed");
  UT_Assert (out.id == 1 && out.score == 99.5f, "popped i1 mismatch");

  C1_Stack_Destroy (&s);
}

static void
test_overflow_underflow (void)
{
  Stack s;
  C1_Stack_Init (&s, sizeof (int), 2);

  int x = 1, y = 2, z = 3;
  UT_Assert (C1_Stack_Push (&s, &x), "push x failed");
  UT_Assert (C1_Stack_Push (&s, &y), "push y failed");
  UT_Assert (!C1_Stack_Push (&s, &z), "should fail to push z (overflow)");

  int out = 0;
  UT_Assert (C1_Stack_Pop (&s, &out), "pop y failed");
  UT_Assert (C1_Stack_Pop (&s, &out), "pop x failed");
  UT_Assert (!C1_Stack_Pop (&s, &out), "should fail to pop (underflow)");

  C1_Stack_Destroy (&s);
}

static void
_unit (void)
{
  test_push_pop_ints ();
  test_push_struct ();
  test_overflow_underflow ();
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "stack/unit");
}
