#include "../include/cryptone/aes.h"

#include "../api/api.h"
#include "../error/error.h"

#include "generic/impl.h"

#include <stdlib.h>
#include <string.h>

WEAK void
AES_Context_Init (AES_Context *ctx, const uint8_t *key, size_t key_len)
{
  switch (key_len)
  {
  case AES128_KEY_BYTES:
  {
    ctx->rndn = 10;
    AES_KeyExpansion128 (key, ctx->rndk);
    break;
  }
  case AES192_KEY_BYTES:
  {
    ctx->rndn = 12;
    AES_KeyExpansion192 (key, ctx->rndk);
    break;
  }
  case AES256_KEY_BYTES:
  {
    ctx->rndn = 14;
    AES_KeyExpansion256 (key, ctx->rndk);
    break;
  }
  default:
    ferroratf (stderr, "/aes/wcontext.c", "unexpected key_len: %zu",
               key_len);
    break;
  }
}

WEAK void
AES_Context_FlushInit (AES_Context *ctx, const uint8_t *key,
                       size_t key_len)
{
  AES_Context_Flush (ctx);
  AES_Context_Init (ctx, key, key_len);
}

WEAK size_t
AES_Context_Size ()
{
  return sizeof (AES_Context);
}

WEAK AES_Context *
AES_Context_Create ()
{
  return (AES_Context *)malloc (AES_Context_Size ());
}

WEAK void
AES_Context_Flush (AES_Context *ctx)
{
  if (ctx)
    memset (ctx, 0, AES_Context_Size ());
}

WEAK void
AES_Context_Destroy (AES_Context *ctx)
{
  free ((void *)ctx);
}
