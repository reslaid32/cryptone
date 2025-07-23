#ifdef __AES__

#include "../../include/cryptone/aes.h"

#include <wmmintrin.h>

void
AES_Context_CBC_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  size_t   blocks     = len / AES_BLOCK_SIZE;
  __m128i *round_keys = (__m128i *)ctx->rndk;
  __m128i  prev_block = _mm_loadu_si128 ((__m128i *)iv);

  for (size_t i = 0; i < blocks; i++)
  {
    __m128i block = _mm_loadu_si128 ((__m128i *)(in + i * AES_BLOCK_SIZE));

    block         = _mm_xor_si128 (block, prev_block);
    block         = _mm_xor_si128 (block, round_keys[0]);

    for (int j = 1; j < ctx->rndn; j++)
      block = _mm_aesenc_si128 (block, round_keys[j]);

    block = _mm_aesenclast_si128 (block, round_keys[ctx->rndn]);

    _mm_storeu_si128 ((__m128i *)(out + i * AES_BLOCK_SIZE), block);
    prev_block = block;
  }

  _mm_storeu_si128 ((__m128i *)iv, prev_block);
}

void
AES_Context_CBC_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  size_t   blocks      = len / AES_BLOCK_SIZE;
  __m128i *round_keys  = (__m128i *)ctx->rndk;
  __m128i  prev_cipher = _mm_loadu_si128 ((__m128i *)iv);

  for (size_t i = 0; i < blocks; i++)
  {
    __m128i cipher_block
        = _mm_loadu_si128 ((__m128i *)(in + i * AES_BLOCK_SIZE));
    __m128i block = cipher_block;

    block         = _mm_xor_si128 (block, round_keys[ctx->rndn]);

    for (int j = ctx->rndn - 1; j > 0; j--)
      block = _mm_aesdec_si128 (block, _mm_aesimc_si128 (round_keys[j]));

    block = _mm_aesdeclast_si128 (block, round_keys[0]);

    block = _mm_xor_si128 (block, prev_cipher);

    _mm_storeu_si128 ((__m128i *)(out + i * AES_BLOCK_SIZE), block);
    prev_cipher = cipher_block;
  }

  _mm_storeu_si128 ((__m128i *)iv, prev_cipher);
}

#endif