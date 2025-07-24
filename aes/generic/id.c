
#include "../../api/api.h"

/* generic */
WEAK int
AES_Impl_Id (void)
{
  return 0;
}

WEAK const char *
AES_Impl_String (void)
{
  return "aes/generic";
}
