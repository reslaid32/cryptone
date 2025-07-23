#ifndef IMPL_H
#define IMPL_H

#include <stdint.h>

uint32_t AES_RotWord (uint32_t w);
uint32_t AES_SubWord (uint32_t w);
void     AES_KeyExpansion128 (const uint8_t *key, uint32_t *w);
void     AES_KeyExpansion192 (const uint8_t *key, uint32_t *w);
void     AES_KeyExpansion256 (const uint8_t *key, uint32_t *w);
void     AES_SubBytes (uint8_t s[4][4]);
void     AES_InvSubBytes (uint8_t s[4][4]);
void     AES_ShiftRows (uint8_t s[4][4]);
void     AES_InvShiftRows (uint8_t s[4][4]);
void     AES_MixColumns (uint8_t s[4][4]);
void     AES_InvMixColumns (uint8_t s[4][4]);
void     AES_AddRoundKey (uint8_t s[4][4], const uint32_t *rk);

#endif