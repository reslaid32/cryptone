#include "../../include/cryptone/aes.h"

#include "helpers.h"

uint32_t
AES_RotWord (uint32_t w)
{
  return (w << 8) | (w >> 24);
}

uint32_t
AES_SubWord (uint32_t w)
{
  return (AES_SBox[(w >> 24) & 0xFF] << 24)
         | (AES_SBox[(w >> 16) & 0xFF] << 16)
         | (AES_SBox[(w >> 8) & 0xFF] << 8) | AES_SBox[w & 0xFF];
}

void
AES_KeyExpansion128 (const uint8_t *key, uint32_t *w)
{
  for (int i = 0; i < 4; ++i)
    w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16)
           | (key[4 * i + 2] << 8) | key[4 * i + 3];

  for (int i = 4; i < 44; ++i)
  {
    uint32_t temp = w[i - 1];
    if (i % 4 == 0)
      temp = AES_SubWord (AES_RotWord (temp)) ^ AES_RCon[(i / 4) - 1];
    w[i] = w[i - 4] ^ temp;
  }
}

void
AES_KeyExpansion192 (const uint8_t *key, uint32_t *w)
{
  for (int i = 0; i < 6; ++i)
    w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16)
           | (key[4 * i + 2] << 8) | key[4 * i + 3];

  for (int i = 6; i < 52; ++i)
  {
    uint32_t temp = w[i - 1];
    if (i % 6 == 0)
      temp = AES_SubWord (AES_RotWord (temp)) ^ AES_RCon[(i / 6) - 1];
    w[i] = w[i - 6] ^ temp;
  }
}

void
AES_KeyExpansion256 (const uint8_t *key, uint32_t *w)
{
  for (int i = 0; i < 8; ++i)
    w[i] = (key[4 * i] << 24) | (key[4 * i + 1] << 16)
           | (key[4 * i + 2] << 8) | key[4 * i + 3];

  for (int i = 8; i < 60; ++i)
  {
    uint32_t temp = w[i - 1];
    if (i % 8 == 0)
      temp = AES_SubWord (AES_RotWord (temp)) ^ AES_RCon[(i / 8) - 1];
    else if (i % 8 == 4)
      temp = AES_SubWord (temp);
    w[i] = w[i - 8] ^ temp;
  }
}

void
AES_SubBytes (uint8_t s[4][4])
{
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      s[i][j] = AES_SBox[s[i][j]];
}

void
AES_InvSubBytes (uint8_t s[4][4])
{
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      s[i][j] = AES_RSBox[s[i][j]];
}

void
AES_ShiftRows (uint8_t s[4][4])
{
  uint8_t tmp;

  tmp     = s[1][0];
  s[1][0] = s[1][1];
  s[1][1] = s[1][2];
  s[1][2] = s[1][3];
  s[1][3] = tmp;
  tmp     = s[2][0];
  s[2][0] = s[2][2];
  s[2][2] = tmp;
  tmp     = s[2][1];
  s[2][1] = s[2][3];
  s[2][3] = tmp;
  tmp     = s[3][3];
  s[3][3] = s[3][2];
  s[3][2] = s[3][1];
  s[3][1] = s[3][0];
  s[3][0] = tmp;
}

void
AES_InvShiftRows (uint8_t s[4][4])
{
  uint8_t tmp;

  tmp     = s[1][3];
  s[1][3] = s[1][2];
  s[1][2] = s[1][1];
  s[1][1] = s[1][0];
  s[1][0] = tmp;
  tmp     = s[2][0];
  s[2][0] = s[2][2];
  s[2][2] = tmp;
  tmp     = s[2][1];
  s[2][1] = s[2][3];
  s[2][3] = tmp;
  tmp     = s[3][0];
  s[3][0] = s[3][1];
  s[3][1] = s[3][2];
  s[3][2] = s[3][3];
  s[3][3] = tmp;
}

void
AES_MixColumns (uint8_t s[4][4])
{
  for (int i = 0; i < 4; ++i)
  {
    uint8_t a = s[0][i], b = s[1][i], c = s[2][i], d = s[3][i];
    s[0][i] = gmul (a, 2) ^ gmul (b, 3) ^ c ^ d;
    s[1][i] = a ^ gmul (b, 2) ^ gmul (c, 3) ^ d;
    s[2][i] = a ^ b ^ gmul (c, 2) ^ gmul (d, 3);
    s[3][i] = gmul (a, 3) ^ b ^ c ^ gmul (d, 2);
  }
}

void
AES_InvMixColumns (uint8_t s[4][4])
{
  for (int i = 0; i < 4; ++i)
  {
    uint8_t a = s[0][i], b = s[1][i], c = s[2][i], d = s[3][i];
    s[0][i] = gmul (a, 0x0e) ^ gmul (b, 0x0b) ^ gmul (c, 0x0d)
              ^ gmul (d, 0x09);
    s[1][i] = gmul (a, 0x09) ^ gmul (b, 0x0e) ^ gmul (c, 0x0b)
              ^ gmul (d, 0x0d);
    s[2][i] = gmul (a, 0x0d) ^ gmul (b, 0x09) ^ gmul (c, 0x0e)
              ^ gmul (d, 0x0b);
    s[3][i] = gmul (a, 0x0b) ^ gmul (b, 0x0d) ^ gmul (c, 0x09)
              ^ gmul (d, 0x0e);
  }
}

void
AES_AddRoundKey (uint8_t s[4][4], const uint32_t *rk)
{
  for (int i = 0; i < 4; ++i)
  {
    uint32_t k = rk[i];
    for (int j = 0; j < 4; ++j)
      s[j][i] ^= (k >> (24 - j * 8)) & 0xFF;
  }
}