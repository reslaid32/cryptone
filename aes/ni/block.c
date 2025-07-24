#ifdef __AES__

#include "../../include/cryptone/aes.h"

#include <assert.h>

#include <wmmintrin.h>

void
AES_Context_EncryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  __m128i  block      = _mm_loadu_si128 ((__m128i *)in);
  __m128i *round_keys = (__m128i *)ctx->rndk;

  block               = _mm_xor_si128 (block, round_keys[0]);

  for (int i = 1; i < ctx->rndn; i++)
    block = _mm_aesenc_si128 (block, round_keys[i]);

  block = _mm_aesenclast_si128 (block, round_keys[ctx->rndn]);

  _mm_storeu_si128 ((__m128i *)out, block);
}

void
AES_Context_DecryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  __m128i  block      = _mm_loadu_si128 ((__m128i *)in);
  __m128i *round_keys = (__m128i *)ctx->rndk;

  block               = _mm_xor_si128 (block, round_keys[ctx->rndn]);

  for (int i = ctx->rndn - 1; i > 0; i--)
    block = _mm_aesdec_si128 (block, _mm_aesimc_si128 (round_keys[i]));

  block = _mm_aesdeclast_si128 (block, round_keys[0]);

  _mm_storeu_si128 ((__m128i *)out, block);
}

#endif