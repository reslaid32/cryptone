#ifndef __AES_IMP_H
#define __AES_IMP_H

#include <stddef.h>
#include <stdint.h>

#include "constants.h"

uint32_t AES_RotWord (uint32_t w);
uint32_t AES_SubWord (uint32_t w);

// -------------------- AES Key Schedule --------------------

void     AES_KeyExpansion128 (const uint8_t *key, uint32_t *w);
void     AES_KeyExpansion256 (const uint8_t *key, uint32_t *w);

// -------------------- AES Transformations --------------------

void     AES_SubBytes (uint8_t s[4][4]);
void     AES_InvSubBytes (uint8_t s[4][4]);

void     AES_ShiftRows (uint8_t s[4][4]);
void     AES_InvShiftRows (uint8_t s[4][4]);

void     AES_MixColumns (uint8_t s[4][4]);

void     AES_InvMixColumns (uint8_t s[4][4]);

void     AES_AddRoundKey (uint8_t s[4][4], const uint32_t *rk);

typedef struct AES_Context
{
  uint32_t rndk[60];
  int      rndn;
} AES_Context;

size_t       AES_Context_Size ();
AES_Context *AES_Context_Create ();
void         AES_Context_Flush (AES_Context *ctx);
void         AES_Context_Destroy (AES_Context *ctx);

void         AES_Context_Init (AES_Context *ctx, const uint8_t *key,
                               size_t key_len);

void         AES_Context_FlushInit (AES_Context *ctx, const uint8_t *key,
                                    size_t key_len);

void         AES_EncryptBlock (const uint8_t *in, uint8_t *out,
                               const uint32_t *rndk, int rndn);

void         AES_DecryptBlock (const uint8_t *in, uint8_t *out,
                               const uint32_t *rndk, int rndn);

void AES_Context_EncryptBlock (const AES_Context *ctx, const uint8_t *in,
                               uint8_t *out);

void AES_Context_DecryptBlock (const AES_Context *ctx, const uint8_t *in,
                               uint8_t *out);

void AES_ECB_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                      const uint32_t *rndk, int rndn);

void AES_ECB_Decrypt (const uint8_t *in, uint8_t *out, size_t len,
                      const uint32_t *rndk, int rndn);

void AES_Context_ECB_Encrypt (const AES_Context *ctx, const uint8_t *in,
                              uint8_t *out, size_t len);

void AES_Context_ECB_Decrypt (const AES_Context *ctx, const uint8_t *in,
                              uint8_t *out, size_t len);

void AES_CBC_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                      uint8_t iv[AES_BLOCK_SIZE], const uint32_t *rndk,
                      int rndn);

void AES_CBC_Decrypt (const uint8_t *in, uint8_t *out, size_t len,
                      uint8_t iv[AES_BLOCK_SIZE], const uint32_t *rndk,
                      int rndn);

void AES_Context_CBC_Encrypt (const AES_Context *ctx, const uint8_t *in,
                              uint8_t *out, size_t len,
                              uint8_t iv[AES_BLOCK_SIZE]);
void AES_Context_CBC_Decrypt (const AES_Context *ctx, const uint8_t *in,
                              uint8_t *out, size_t len,
                              uint8_t iv[AES_BLOCK_SIZE]);

void AES_CTR_Encrypt (const uint8_t *in, uint8_t *out, size_t len,
                      uint8_t nonce[AES_BLOCK_SIZE], const uint32_t *rndk,
                      int rndn);

void AES_Context_CTR_Encrypt (const AES_Context *ctx, const uint8_t *in,
                              uint8_t *out, size_t len,
                              uint8_t nonce[AES_BLOCK_SIZE]);

#endif
