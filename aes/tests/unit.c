#include "../../unit/unit.h"
#include "../aes.h"

#include <stdio.h>
#include <string.h>

#define ARR_EQ(a, b, len)                                                 \
  UT_Assert (memcmp ((a), (b), (len)) == 0, "Arrays must be equal")

static void
test_rotword (void)
{
  uint32_t in  = 0x12345678;
  uint32_t out = AES_RotWord (in);
  UT_Assert (out == 0x34567812, "AES_RotWord output mismatch");
}

static void
test_subword (void)
{
  uint32_t in  = 0x00102030;
  uint32_t out = AES_SubWord (in);
  UT_Assert (out != in, "AES_SubWord should change input");
}

static void
test_key_expansion_128 (void)
{
  uint8_t  key[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
                       0xab, 0xf7, 0x97, 0x75, 0x13, 0x11, 0x8e, 0x9a };
  uint32_t w[44];
  AES_KeyExpansion128 (key, w);
  UT_Assert (w[0] == 0x2b7e1516, "KeyExpansion128 first word mismatch");
  UT_Assert (w[43] != 0, "KeyExpansion128 last word must not be zero");
}

static void
test_key_expansion_256 (void)
{
  uint8_t key[32]
      = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae,
          0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61,
          0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
  uint32_t w[60];
  AES_KeyExpansion256 (key, w);
  UT_Assert (w[0] == 0x603deb10, "KeyExpansion256 first word mismatch");
  UT_Assert (w[59] != 0, "KeyExpansion256 last word must not be zero");
}

static void
test_addroundkey (void)
{
  uint8_t state[4][4] = {
    { 0x32, 0x88, 0x31, 0xe0 },
    { 0x43, 0x5a, 0x31, 0x37 },
    { 0xf6, 0x30, 0x98, 0x07 },
    { 0xa8, 0x8d, 0xa2, 0x34 },
  };
  uint32_t rk[4] = { 0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c };
  AES_AddRoundKey (state, rk);
  UT_Assert (state[0][0] == (0x32 ^ 0x2b), "AddRoundKey result mismatch");
}

static void
test_subbytes_inverse (void)
{
  uint8_t state[4][4] = { { 0 } };
  state[0][0]         = 0x53;
  AES_SubBytes (state);
  AES_InvSubBytes (state);
  UT_Assert (state[0][0] == 0x53, "SubBytes/InvSubBytes inversion failed");
}

static void
test_shiftrows_inverse (void)
{
  uint8_t state[4][4] = {
    {  0,  1,  2,  3 },
    {  4,  5,  6,  7 },
    {  8,  9, 10, 11 },
    { 12, 13, 14, 15 },
  };
  AES_ShiftRows (state);
  AES_InvShiftRows (state);
  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      UT_Assert (state[j][i] == (j * 4 + i),
                 "ShiftRows/InvShiftRows inversion failed");
}

static void
test_mixcolumns_inverse (void)
{
  uint8_t original[4][4] = {
    { 0xdb, 0x13, 0x53, 0x45 },
    { 0xf2, 0x0a, 0x22, 0x5c },
    { 0x01, 0x01, 0x01, 0x01 },
    { 0xc6, 0xc6, 0xc6, 0xc6 },
  };
  uint8_t state[4][4];
  memcpy (state, original, sizeof (state));
  AES_MixColumns (state);
  AES_InvMixColumns (state);
  ARR_EQ (state, original, sizeof (state));
}

static void
test_encrypt_decrypt_block (void)
{
  AES_Context ctx;
  uint8_t     key[16]   = { 0 };
  uint8_t     plain[16] = { 0 };
  uint8_t     enc[16], dec[16];
  AES_Context_Init (&ctx, key, sizeof (key));
  AES_Context_EncryptBlock (&ctx, plain, enc);
  AES_Context_DecryptBlock (&ctx, enc, dec);
  ARR_EQ (plain, dec, 16);
}

static void
_unit (void)
{
  __UT_PRINTF ("Running AES low-level function tests...\n");
  test_rotword ();
  test_subword ();
  test_key_expansion_128 ();
  test_key_expansion_256 ();
  test_addroundkey ();
  test_subbytes_inverse ();
  test_shiftrows_inverse ();
  test_mixcolumns_inverse ();
  test_encrypt_decrypt_block ();
  __UT_PRINTF ("AES low-level function tests passed.\n");
}

__attribute__ ((constructor (200))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "aes/unit");
}
