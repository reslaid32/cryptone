#ifdef __AES__

#include "../../include/cryptone/aes.h"

#include <string.h>
#include <wmmintrin.h>

void
AES_Context_CTR_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t nonce[AES_BLOCK_SIZE])
{
  uint8_t counter_buf[AES_BLOCK_SIZE];
  memcpy (counter_buf, nonce, AES_BLOCK_SIZE);

  size_t blocks    = len / AES_BLOCK_SIZE;
  size_t remaining = len % AES_BLOCK_SIZE;

  for (size_t i = 0; i < blocks; i++)
  {
    __m128i counter = _mm_loadu_si128 ((const __m128i *)counter_buf);

    __m128i ks      = counter;
    ks              = _mm_xor_si128 (ks,
                                     _mm_loadu_si128 ((const __m128i *)&ctx->rndk[0]));

    for (int r = 1; r < ctx->rndn; r++)
    {
      ks = _mm_aesenc_si128 (
          ks, _mm_loadu_si128 ((const __m128i *)&ctx->rndk[4 * r]));
    }
    ks = _mm_aesenclast_si128 (
        ks, _mm_loadu_si128 ((const __m128i *)&ctx->rndk[4 * ctx->rndn]));

    __m128i pt
        = _mm_loadu_si128 ((const __m128i *)(in + i * AES_BLOCK_SIZE));
    __m128i ct = _mm_xor_si128 (pt, ks);
    _mm_storeu_si128 ((__m128i *)(out + i * AES_BLOCK_SIZE), ct);

    for (int b = AES_BLOCK_SIZE - 1; b >= 0; b--)
      if (++counter_buf[b] != 0)
        break;
  }

  if (remaining)
  {
    __m128i counter = _mm_loadu_si128 ((const __m128i *)counter_buf);
    __m128i ks      = _mm_xor_si128 (
        counter, _mm_loadu_si128 ((const __m128i *)&ctx->rndk[0]));
    for (int r = 1; r < ctx->rndn; r++)
    {
      ks = _mm_aesenc_si128 (
          ks, _mm_loadu_si128 ((const __m128i *)&ctx->rndk[4 * r]));
    }
    ks = _mm_aesenclast_si128 (
        ks, _mm_loadu_si128 ((const __m128i *)&ctx->rndk[4 * ctx->rndn]));

    uint8_t ks_bytes[AES_BLOCK_SIZE];
    _mm_storeu_si128 ((__m128i *)ks_bytes, ks);

    size_t offset = blocks * AES_BLOCK_SIZE;
    for (size_t i = 0; i < remaining; i++)
      out[offset + i] = in[offset + i] ^ ks_bytes[i];

    for (int b = AES_BLOCK_SIZE - 1; b >= 0; b--)
      if (++counter_buf[b] != 0)
        break;
  }

  memcpy (nonce, counter_buf, AES_BLOCK_SIZE);
}

#endif