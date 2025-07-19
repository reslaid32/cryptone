#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aes.h"
#include "constants.h"
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

// -------------------- AES Key Schedule --------------------

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

// -------------------- AES Transformations --------------------

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

// -------------------- AES Context --------------------

void
AES_Context_Init (AES_Context *ctx, const uint8_t *key, size_t key_len)
{
  if (key_len == AES128_KEY_BYTES)
  {
    ctx->rndn = 10;
    AES_KeyExpansion128 (key, ctx->rndk);
  }
  else if (key_len == AES256_KEY_BYTES)
  {
    ctx->rndn = 14;
    AES_KeyExpansion256 (key, ctx->rndk);
  }
  else
  {
    fprintf (stderr, "Unsupported key length: %zu\n", key_len);
    exit (1);
  }
}

void
AES_Context_FlushInit (AES_Context *ctx, const uint8_t *key,
                       size_t key_len)
{
  AES_Context_Flush (ctx);
  AES_Context_Init (ctx, key, key_len);
}

size_t
AES_Context_Size ()
{
  return sizeof (AES_Context);
}

AES_Context *
AES_Context_Create ()
{
  return (AES_Context *)malloc (AES_Context_Size ());
}

void
AES_Context_Flush (AES_Context *ctx)
{
  if (ctx)
    memset (ctx, 0, AES_Context_Size ());
}

void
AES_Context_Destroy (AES_Context *ctx)
{
  free ((void *)ctx);
}

// -------------------- AES Encrypt/Decrypt one block --------------------

void
AES_EncryptBlock (const uint8_t *in, uint8_t *out, const uint32_t *rndk,
                  int rndn)
{
  uint8_t s[4][4];
  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    s[i % 4][i / 4] = in[i];

  AES_AddRoundKey (s, &rndk[0]);

  for (int r = 1; r < rndn; ++r)
  {
    AES_SubBytes (s);
    AES_ShiftRows (s);
    AES_MixColumns (s);
    AES_AddRoundKey (s, &rndk[r * 4]);
  }

  AES_SubBytes (s);
  AES_ShiftRows (s);
  AES_AddRoundKey (s, &rndk[rndn * 4]);

  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    out[i] = s[i % 4][i / 4];
}

void
AES_DecryptBlock (const uint8_t *in, uint8_t *out, const uint32_t *rndk,
                  int rndn)
{
  uint8_t s[4][4];
  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    s[i % 4][i / 4] = in[i];

  AES_AddRoundKey (s, &rndk[rndn * 4]);

  for (int r = rndn - 1; r > 0; --r)
  {
    AES_InvShiftRows (s);
    AES_InvSubBytes (s);
    AES_AddRoundKey (s, &rndk[r * 4]);
    AES_InvMixColumns (s);
  }

  AES_InvShiftRows (s);
  AES_InvSubBytes (s);
  AES_AddRoundKey (s, &rndk[0]);

  for (int i = 0; i < AES_BLOCK_SIZE; ++i)
    out[i] = s[i % 4][i / 4];
}

void
AES_Context_EncryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  return AES_EncryptBlock (in, out, ctx->rndk, ctx->rndn);
}

void
AES_Context_DecryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  return AES_DecryptBlock (in, out, ctx->rndk, ctx->rndn);
}

void
AES_ECB_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                 const uint32_t *rndk, int rndn)
{
  assert (len % AES_BLOCK_SIZE == 0);
  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
    AES_EncryptBlock (in + i, out + i, rndk, rndn);
}

void
AES_ECB_Decrypt (const uint8_t *in, uint8_t *out, size_t len,
                 const uint32_t *rndk, int rndn)
{
  assert (len % AES_BLOCK_SIZE == 0);
  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
    AES_DecryptBlock (in + i, out + i, rndk, rndn);
}

void
AES_Context_ECB_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  return AES_ECB_Encrypt (in, out, len, ctx->rndk, ctx->rndn);
}

void
AES_Context_ECB_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  return AES_ECB_Decrypt (in, out, len, ctx->rndk, ctx->rndn);
}

void
AES_CBC_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                 uint8_t iv[AES_BLOCK_SIZE], const uint32_t *rndk,
                 int rndn)
{
  assert (len % AES_BLOCK_SIZE == 0);
  uint8_t block[AES_BLOCK_SIZE];

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    for (int j = 0; j < AES_BLOCK_SIZE; ++j)
      block[j] = in[i + j] ^ iv[j];

    AES_EncryptBlock (block, out + i, rndk, rndn);
    memcpy (iv, out + i, AES_BLOCK_SIZE);
  }
}

void
AES_CBC_Decrypt (const uint8_t *in, uint8_t *out, size_t len,
                 uint8_t iv[AES_BLOCK_SIZE], const uint32_t *rndk,
                 int rndn)
{
  assert (len % AES_BLOCK_SIZE == 0);
  uint8_t block[AES_BLOCK_SIZE];

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    memcpy (block, in + i, AES_BLOCK_SIZE);
    AES_DecryptBlock (in + i, out + i, rndk, rndn);

    for (int j = 0; j < AES_BLOCK_SIZE; ++j)
      out[i + j] ^= iv[j];

    memcpy (iv, block, AES_BLOCK_SIZE);
  }
}

void
AES_Context_CBC_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  return AES_CBC_Encrypt (in, out, len, iv, ctx->rndk, ctx->rndn);
}

void
AES_Context_CBC_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  return AES_CBC_Decrypt (in, out, len, iv, ctx->rndk, ctx->rndn);
}

void
AES_CTR_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                 uint8_t nonce[AES_BLOCK_SIZE], const uint32_t *rndk,
                 int rndn)
{
  uint8_t stream_block[AES_BLOCK_SIZE];
  uint8_t counter[AES_BLOCK_SIZE];
  memcpy (counter, nonce, AES_BLOCK_SIZE);

  for (size_t i = 0; i < len; i += AES_BLOCK_SIZE)
  {
    AES_EncryptBlock (counter, stream_block, rndk, rndn);
    size_t block_size
        = (len - i < AES_BLOCK_SIZE) ? len - i : AES_BLOCK_SIZE;

    for (size_t j = 0; j < block_size; ++j)
      out[i + j] = in[i + j] ^ stream_block[j];

    for (int j = 15; j >= 0; --j)
      if (++counter[j])
        break;
  }
}

void
AES_Context_CTR_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t nonce[AES_BLOCK_SIZE])
{
  return AES_CTR_Encrypt (in, out, len, nonce, ctx->rndk, ctx->rndn);
}
