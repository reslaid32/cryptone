#ifdef __AES__

#include "../../include/cryptone/aes.h"
#include "impl.h"

#include "../../error/error.h"
#include <stdio.h>

void
AES_Context_Init (AES_Context *ctx, const uint8_t *key, size_t key_len)
{
  __m128i *round_keys = (__m128i *)ctx->rndk;

  switch (key_len)
  {
  case AES128_KEY_BYTES:
  {
    ctx->rndn       = 10;
    __m128i key_128 = _mm_loadu_si128 ((__m128i *)key);
    round_keys[0]   = key_128;

    round_keys[1]   = AESNI_KeyExpansion128 (
        round_keys[0], _mm_aeskeygenassist_si128 (round_keys[0], 0x01));
    round_keys[2] = AESNI_KeyExpansion128 (
        round_keys[1], _mm_aeskeygenassist_si128 (round_keys[1], 0x02));
    round_keys[3] = AESNI_KeyExpansion128 (
        round_keys[2], _mm_aeskeygenassist_si128 (round_keys[2], 0x04));
    round_keys[4] = AESNI_KeyExpansion128 (
        round_keys[3], _mm_aeskeygenassist_si128 (round_keys[3], 0x08));
    round_keys[5] = AESNI_KeyExpansion128 (
        round_keys[4], _mm_aeskeygenassist_si128 (round_keys[4], 0x10));
    round_keys[6] = AESNI_KeyExpansion128 (
        round_keys[5], _mm_aeskeygenassist_si128 (round_keys[5], 0x20));
    round_keys[7] = AESNI_KeyExpansion128 (
        round_keys[6], _mm_aeskeygenassist_si128 (round_keys[6], 0x40));
    round_keys[8] = AESNI_KeyExpansion128 (
        round_keys[7], _mm_aeskeygenassist_si128 (round_keys[7], 0x80));
    round_keys[9] = AESNI_KeyExpansion128 (
        round_keys[8], _mm_aeskeygenassist_si128 (round_keys[8], 0x1B));
    round_keys[10] = AESNI_KeyExpansion128 (
        round_keys[9], _mm_aeskeygenassist_si128 (round_keys[9], 0x36));
    break;
  }

  case AES192_KEY_BYTES:
  {
    ctx->rndn     = 12;
    __m128i key1  = _mm_loadu_si128 ((__m128i *)key);
    __m128i key2  = _mm_loadl_epi64 ((__m128i *)(key + 16));

    round_keys[0] = key1;
    round_keys[1] = key2;

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x01));
    round_keys[1] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (round_keys[1]), _mm_castsi128_ps (key1), 0x45));
    round_keys[2] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (key1), _mm_castsi128_ps (key2), 0x4E));

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x02));
    round_keys[3] = key1;
    round_keys[4] = key2;

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x04));
    round_keys[4] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (round_keys[4]), _mm_castsi128_ps (key1), 0x45));
    round_keys[5] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (key1), _mm_castsi128_ps (key2), 0x4E));

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x08));
    round_keys[6] = key1;
    round_keys[7] = key2;

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x10));
    round_keys[7] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (round_keys[7]), _mm_castsi128_ps (key1), 0x45));
    round_keys[8] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (key1), _mm_castsi128_ps (key2), 0x4E));

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x20));
    round_keys[9]  = key1;
    round_keys[10] = key2;

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x40));
    round_keys[10] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (round_keys[10]), _mm_castsi128_ps (key1), 0x45));
    round_keys[11] = _mm_castps_si128 (_mm_shuffle_ps (
        _mm_castsi128_ps (key1), _mm_castsi128_ps (key2), 0x4E));

    AESNI_KeyExpansion192 (&key1, &key2,
                           _mm_aeskeygenassist_si128 (key2, 0x80));
    round_keys[12] = key1;
    break;
  }

  case AES256_KEY_BYTES:
  {
    ctx->rndn     = 14;
    __m128i key1  = _mm_loadu_si128 ((__m128i *)key);
    __m128i key2  = _mm_loadu_si128 ((__m128i *)(key + 16));

    round_keys[0] = key1;
    round_keys[1] = key2;

    round_keys[2] = AESNI_KeyExpansion256_1 (
        round_keys[0], _mm_aeskeygenassist_si128 (round_keys[1], 0x01));
    round_keys[3] = AESNI_KeyExpansion256_2 (
        round_keys[1], _mm_aeskeygenassist_si128 (round_keys[2], 0x00));

    round_keys[4] = AESNI_KeyExpansion256_1 (
        round_keys[2], _mm_aeskeygenassist_si128 (round_keys[3], 0x02));
    round_keys[5] = AESNI_KeyExpansion256_2 (
        round_keys[3], _mm_aeskeygenassist_si128 (round_keys[4], 0x00));

    round_keys[6] = AESNI_KeyExpansion256_1 (
        round_keys[4], _mm_aeskeygenassist_si128 (round_keys[5], 0x04));
    round_keys[7] = AESNI_KeyExpansion256_2 (
        round_keys[5], _mm_aeskeygenassist_si128 (round_keys[6], 0x00));

    round_keys[8] = AESNI_KeyExpansion256_1 (
        round_keys[6], _mm_aeskeygenassist_si128 (round_keys[7], 0x08));
    round_keys[9] = AESNI_KeyExpansion256_2 (
        round_keys[7], _mm_aeskeygenassist_si128 (round_keys[8], 0x00));

    round_keys[10] = AESNI_KeyExpansion256_1 (
        round_keys[8], _mm_aeskeygenassist_si128 (round_keys[9], 0x10));
    round_keys[11] = AESNI_KeyExpansion256_2 (
        round_keys[9], _mm_aeskeygenassist_si128 (round_keys[10], 0x00));

    round_keys[12] = AESNI_KeyExpansion256_1 (
        round_keys[10], _mm_aeskeygenassist_si128 (round_keys[11], 0x20));
    round_keys[13] = AESNI_KeyExpansion256_2 (
        round_keys[11], _mm_aeskeygenassist_si128 (round_keys[12], 0x00));

    round_keys[14] = AESNI_KeyExpansion256_1 (
        round_keys[12], _mm_aeskeygenassist_si128 (round_keys[13], 0x40));
    break;
  }

  default:
    ferroratf (stderr, "/aes/ni/context.c", "unexpected key_len: %zu",
               key_len);
    break;
  }
}

#endif