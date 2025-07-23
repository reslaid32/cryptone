#include "../../api/api.h"
#include "../../include/cryptone/aes.h"

#include "impl.h"
#include <assert.h>

WEAK void
AES_Context_EncryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  uint8_t s[4][4];
  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    s[i % 4][i / 4] = in[i];

  AES_AddRoundKey (s, &ctx->rndk[0]);

  for (int r = 1; r < ctx->rndn; ++r)
  {
    AES_SubBytes (s);
    AES_ShiftRows (s);
    AES_MixColumns (s);
    AES_AddRoundKey (s, &ctx->rndk[r * 4]);
  }

  AES_SubBytes (s);
  AES_ShiftRows (s);
  AES_AddRoundKey (s, &ctx->rndk[ctx->rndn * 4]);

  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    out[i] = s[i % 4][i / 4];
}

WEAK void
AES_Context_DecryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  uint8_t s[4][4];
  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    s[i % 4][i / 4] = in[i];

  AES_AddRoundKey (s, &ctx->rndk[ctx->rndn * 4]);

  for (int r = ctx->rndn - 1; r > 0; --r)
  {
    AES_InvShiftRows (s);
    AES_InvSubBytes (s);
    AES_AddRoundKey (s, &ctx->rndk[r * 4]);
    AES_InvMixColumns (s);
  }

  AES_InvShiftRows (s);
  AES_InvSubBytes (s);
  AES_AddRoundKey (s, &ctx->rndk[0]);

  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    out[i] = s[i % 4][i / 4];
}
