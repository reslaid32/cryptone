#include "../api/api.h"
#include "../unit/unit.h"

WEAK void
UT_Register (unit_test_fn fn, const char *name)
{
  (void)fn;
  (void)name;
}

WEAK int
UT_RunAll (void)
{
  return 0;
}

WEAK void
UT_BasicAssert (int condition, const char *cond_str, const char *msg)
{
  (void)condition;
  (void)cond_str;
  (void)msg;
}