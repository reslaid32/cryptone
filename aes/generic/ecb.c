#include "../../api/api.h"
#include "../../include/cryptone/aes.h"

#include <assert.h>

WEAK void
AES_Context_ECB_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  assert (len % AES_BLOCK_SIZE == 0);
  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
    AES_Context_EncryptBlock (ctx, in + i, out + i);
}

WEAK void
AES_Context_ECB_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  assert (len % AES_BLOCK_SIZE == 0);
  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
    AES_Context_DecryptBlock (ctx, in + i, out + i);
}
