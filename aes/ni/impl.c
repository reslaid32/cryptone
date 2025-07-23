#ifdef __AES__

#include "impl.h"

__m128i
AESNI_KeyExpansion128 (__m128i key, __m128i keygened)
{
  keygened = _mm_shuffle_epi32 (keygened, 0xFF);
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  return _mm_xor_si128 (key, keygened);
}

void
AESNI_KeyExpansion192 (__m128i *key1, __m128i *key2, __m128i keygened)
{
  __m128i tmp1, tmp2, tmp3;

  keygened = _mm_shuffle_epi32 (keygened, 0x55);
  tmp1     = _mm_slli_si128 (*key1, 4);
  *key1    = _mm_xor_si128 (*key1, tmp1);
  tmp1     = _mm_slli_si128 (tmp1, 4);
  *key1    = _mm_xor_si128 (*key1, tmp1);
  tmp1     = _mm_slli_si128 (tmp1, 4);
  *key1    = _mm_xor_si128 (*key1, tmp1);
  *key1    = _mm_xor_si128 (*key1, keygened);

  tmp2     = _mm_slli_si128 (*key1, 4);
  tmp3     = _mm_srli_si128 (*key2, 12);
  tmp2     = _mm_xor_si128 (tmp2, tmp3);
  *key2    = _mm_xor_si128 (*key2, _mm_slli_si128 (*key2, 4));
  *key2    = _mm_xor_si128 (*key2, tmp2);
}

__m128i
AESNI_KeyExpansion256_1 (__m128i key, __m128i keygened)
{
  keygened = _mm_shuffle_epi32 (keygened, 0xFF);
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  return _mm_xor_si128 (key, keygened);
}

__m128i
AESNI_KeyExpansion256_2 (__m128i key, __m128i keygened)
{
  keygened = _mm_shuffle_epi32 (keygened, 0xAA);
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  key      = _mm_xor_si128 (key, _mm_slli_si128 (key, 4));
  return _mm_xor_si128 (key, keygened);
}

#endif