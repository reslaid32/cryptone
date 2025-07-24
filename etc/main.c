#include "../include/cryptone/aes.h"
#include "../log/log.h"
#include "../unit/unit.h"

int
main (void)
{
  log_set_stream (stderr);
  log_debug ("aes = %s", AES_Impl_String ());

  UT_RunAll ();

  return 0;
}