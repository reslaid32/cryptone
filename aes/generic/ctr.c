#include "../../api/api.h"
#include "../../include/cryptone/aes.h"

#include <string.h>

WEAK void
AES_Context_CTR_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t nonce[AES_BLOCK_SIZE])
{
  uint8_t stream_block[AES_BLOCK_SIZE];
  uint8_t counter[AES_BLOCK_SIZE];
  memcpy (counter, nonce, AES_BLOCK_SIZE);

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    AES_Context_EncryptBlock (ctx, counter, stream_block);
    size_t block_size
        = (len - i < AES_BLOCK_SIZE) ? len - i : AES_BLOCK_SIZE;

    for (size_t j = 0; j < block_size; ++j)
      out[i + j] = in[i + j] ^ stream_block[j];

    for (int j = 15; j >= 0; --j)
      if (++counter[j])
        break;
  }
}
