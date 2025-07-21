#include "../unit/unit.h"

__attribute__ ((weak)) void
UT_Register (unit_test_fn fn, const char *name)
{
  (void)fn;
  (void)name;
}

__attribute__ ((weak)) int
UT_RunAll (void)
{
  return 0;
}

__attribute__ ((weak)) void
UT_BasicAssert (int condition, const char *cond_str, const char *msg)
{
  (void)condition;
  (void)cond_str;
  (void)msg;
}