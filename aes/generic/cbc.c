#include "../../api/api.h"
#include "../../include/cryptone/aes.h"

#include <assert.h>
#include <string.h>

WEAK void
AES_Context_CBC_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  assert (len % AES_BLOCK_SIZE == 0);
  uint8_t block[AES_BLOCK_SIZE];

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    for (int j = 0; j < AES_BLOCK_SIZE; ++j)
      block[j] = in[i + j] ^ iv[j];

    AES_Context_EncryptBlock (ctx, block, out + i);
    memcpy (iv, out + i, AES_BLOCK_SIZE);
  }
}

WEAK void
AES_Context_CBC_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  assert (len % AES_BLOCK_SIZE == 0);
  uint8_t block[AES_BLOCK_SIZE];

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    memcpy (block, in + i, AES_BLOCK_SIZE);
    AES_Context_DecryptBlock (ctx, in + i, out + i);

    for (int j = 0; j < AES_BLOCK_SIZE; ++j)
      out[i + j] ^= iv[j];

    memcpy (iv, block, AES_BLOCK_SIZE);
  }
}
