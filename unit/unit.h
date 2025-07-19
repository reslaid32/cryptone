#ifndef __UNIT_IMP_H
#define __UNIT_IMP_H

#ifdef __UT_NO_FPRINTF
#define __UT_FPRINTF(...)
#endif

#ifndef __UT_FPRINTF
#define __UT_FPRINTF(...) fprintf (__VA_ARGS__)
#endif

#define __UT_PRINTF(...) __UT_FPRINTF (stderr, __VA_ARGS__)

typedef void (*unit_test_fn) (void);

void UT_Register (unit_test_fn fn, const char *name);
int  UT_RunAll (void);

#define UT_Assert(cond, msg) UT_BasicAssert (cond, #cond, msg)
void UT_BasicAssert (int condition, const char *cond_str, const char *msg);

#endif
