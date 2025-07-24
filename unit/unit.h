#ifndef __UNIT_IMP_H
#define __UNIT_IMP_H

typedef void (*unit_test_fn) (void);

void UT_Register (unit_test_fn fn, const char *name);
int  UT_RunAll (void);

#define UT_Assert(cond, msg) UT_BasicAssert (cond, #cond, msg)
void UT_BasicAssert (int condition, const char *cond_str, const char *msg);

#endif
