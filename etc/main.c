#include "../include/cryptone/aes.h"
#include "../log/log.h"
#include "../unit/unit.h"

int
main (void)
{
  LOG_SetAutoNewline (1);
  LOG_Info ("aes = %s", AES_Impl_String ());

  LOG_SetLevel (LOG_VERBOSE);
  int log_enabled =
#ifndef __UT_QUIET
      1
#else
      0
#endif
      ;
  LOG_Enable (log_enabled);

  UT_RunAll ();

  return 0;
}