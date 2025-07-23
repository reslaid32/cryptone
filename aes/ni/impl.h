#ifndef IMPL_H
#define IMPL_H

#include <wmmintrin.h>

__m128i AESNI_KeyExpansion128 (__m128i key, __m128i keygened);
void    AESNI_KeyExpansion192 (__m128i *key1, __m128i *key2,
                               __m128i keygened);
__m128i AESNI_KeyExpansion256_1 (__m128i key, __m128i keygened);
__m128i AESNI_KeyExpansion256_2 (__m128i key, __m128i keygened);

#endif