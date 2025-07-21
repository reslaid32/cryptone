#include "../../unit/unit.h"

#include "../../include/cryptone/aes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __UT_AES_VALIDATION_LCRYPTO
#include <openssl/evp.h>
#endif

static void
test_ecb (AES_Context *ctx, uint8_t *input, size_t len, uint8_t *key,
          size_t key_len)
{
  AES_Context_Flush (ctx);

  uint8_t *enc = malloc (len);
  uint8_t *dec = malloc (len);

  AES_Context_Init (ctx, key, key_len);
  AES_Context_ECB_Encrypt (ctx, input, enc, len);
  AES_Context_ECB_Decrypt (ctx, enc, dec, len);

#ifdef __UT_AES_VALIDATION_LCRYPTO
  uint8_t          *ossl_enc = malloc (len);
  EVP_CIPHER_CTX   *ectx     = EVP_CIPHER_CTX_new ();
  const EVP_CIPHER *cipher
      = (key_len == 16) ? EVP_aes_128_ecb () : EVP_aes_256_ecb ();

  EVP_EncryptInit_ex (ectx, cipher, NULL, key, NULL);
  EVP_CIPHER_CTX_set_padding (ectx, 0);

  int out_len   = 0;
  int total_len = 0;
  EVP_EncryptUpdate (ectx, ossl_enc, &out_len, input, (int)len);
  total_len += out_len;

  EVP_EncryptFinal_ex (ectx, ossl_enc + total_len, &out_len);
  EVP_CIPHER_CTX_free (ectx);

  UT_Assert (memcmp (enc, ossl_enc, len) == 0,
             "AES ECB encryption must match OpenSSL");

  free (ossl_enc);
#endif

  __UT_PRINTF ("Encrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", enc[i]);
  __UT_PRINTF ("\n");

  __UT_PRINTF ("Decrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", dec[i]);
  __UT_PRINTF ("\n");

  __UT_PRINTF ("ECB Input:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", input[i]);
  __UT_PRINTF ("\n");

  UT_Assert (memcmp (input, dec, len) == 0,
             "AES ECB decrypt must match input");

  free (enc);
  free (dec);
}

static void
test_cbc (AES_Context *ctx, uint8_t *input, size_t len, uint8_t *key,
          size_t key_len, uint8_t iv_orig[16])
{
  AES_Context_Flush (ctx);

  uint8_t *enc = malloc (len);
  uint8_t *dec = malloc (len);
  uint8_t  iv_enc[16], iv_dec[16];

  AES_Context_Init (ctx, key, key_len);

  memcpy (iv_enc, iv_orig, 16);
  AES_Context_CBC_Encrypt (ctx, input, enc, len, iv_enc);

#ifdef __UT_AES_VALIDATION_LCRYPTO
  uint8_t *ossl_enc = malloc (len);
  uint8_t  iv2[16];
  memcpy (iv2, iv_orig, 16);

  EVP_CIPHER_CTX   *ectx = EVP_CIPHER_CTX_new ();
  const EVP_CIPHER *cipher
      = (key_len == 16) ? EVP_aes_128_cbc () : EVP_aes_256_cbc ();

  EVP_EncryptInit_ex (ectx, cipher, NULL, key, iv2);
  EVP_CIPHER_CTX_set_padding (ectx, 0);

  int out_len = 0, total_len = 0;
  EVP_EncryptUpdate (ectx, ossl_enc, &out_len, input, (int)len);
  total_len += out_len;
  EVP_EncryptFinal_ex (ectx, ossl_enc + total_len, &out_len);
  EVP_CIPHER_CTX_free (ectx);

  UT_Assert (memcmp (enc, ossl_enc, len) == 0,
             "AES CBC encryption must match OpenSSL");

  free (ossl_enc);
#endif

  memcpy (iv_dec, iv_orig, 16);
  AES_Context_CBC_Decrypt (ctx, enc, dec, len, iv_dec);

  __UT_PRINTF ("CBC Encrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", enc[i]);
  __UT_PRINTF ("\nCBC Decrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", dec[i]);
  __UT_PRINTF ("\nCBC Input:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", input[i]);
  __UT_PRINTF ("\n");

  UT_Assert (memcmp (input, dec, len) == 0,
             "AES CBC decrypt must match input");

  free (enc);
  free (dec);
}

static void
test_ctr (AES_Context *ctx, uint8_t *input, size_t len, uint8_t *key,
          size_t key_len, uint8_t nonce[16])
{
  AES_Context_Flush (ctx);

  uint8_t *enc = malloc (len);
  uint8_t *dec = malloc (len);

  AES_Context_Init (ctx, key, key_len);
  AES_Context_CTR_Encrypt (ctx, input, enc, len, nonce);
  AES_Context_CTR_Encrypt (ctx, enc, dec, len, nonce);

#ifdef __UT_AES_VALIDATION_LCRYPTO
  uint8_t *ossl_enc = malloc (len);
  uint8_t  nonce2[16];
  memcpy (nonce2, nonce, 16);

  EVP_CIPHER_CTX   *ectx = EVP_CIPHER_CTX_new ();
  const EVP_CIPHER *cipher
      = (key_len == 16) ? EVP_aes_128_ctr () : EVP_aes_256_ctr ();

  EVP_EncryptInit_ex (ectx, cipher, NULL, key, nonce2);
  EVP_CIPHER_CTX_set_padding (ectx, 0);

  int out_len = 0, total_len = 0;
  EVP_EncryptUpdate (ectx, ossl_enc, &out_len, input, (int)len);
  total_len += out_len;
  EVP_EncryptFinal_ex (ectx, ossl_enc + total_len, &out_len);
  EVP_CIPHER_CTX_free (ectx);

  UT_Assert (memcmp (enc, ossl_enc, len) == 0,
             "AES CTR encryption must match OpenSSL");

  free (ossl_enc);
#endif

  __UT_PRINTF ("CTR Encrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", enc[i]);
  __UT_PRINTF ("\n");

  __UT_PRINTF ("CTR Decrypted:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", dec[i]);
  __UT_PRINTF ("\n");

  __UT_PRINTF ("CTR Input:\n");
  for (size_t i = 0; i < len; ++i)
    __UT_PRINTF ("%02x ", input[i]);
  __UT_PRINTF ("\n");

  UT_Assert (memcmp (input, dec, len) == 0,
             "AES CTR decrypt must match input");

  free (enc);
  free (dec);
}

static void
_unit (void)
{
  uint8_t key128[AES128_KEY_BYTES] = {
    0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe,
    0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
  };

  uint8_t key256[AES256_KEY_BYTES]
      = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae,
          0xf0, 0x85, 0x7d, 0x77, 0x81, 0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61,
          0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };

  uint8_t input[16] = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96,
                        0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a };

  uint8_t iv[16]    = { 0 };
  uint8_t nonce[16] = { 0 };

  AES_Context *ctx  = AES_Context_Create ();

  __UT_PRINTF ("=== AES-128 ===\n");
  __UT_PRINTF ("\nECB mode:\n");
  test_ecb (ctx, input, sizeof (input), key128, sizeof (key128));

  __UT_PRINTF ("\nCBC mode:\n");
  test_cbc (ctx, input, sizeof (input), key128, sizeof (key128), iv);

  __UT_PRINTF ("\nCTR mode:\n");
  test_ctr (ctx, input, sizeof (input), key128, sizeof (key128), nonce);

  __UT_PRINTF ("\n=== AES-256 ===\n");
  __UT_PRINTF ("\nECB mode:\n");
  test_ecb (ctx, input, sizeof (input), key256, sizeof (key256));

  __UT_PRINTF ("\nCBC mode:\n");
  test_cbc (ctx, input, sizeof (input), key256, sizeof (key256), iv);

  __UT_PRINTF ("\nCTR mode:\n");
  test_ctr (ctx, input, sizeof (input), key256, sizeof (key256), nonce);

  AES_Context_Destroy (ctx);
}

__attribute__ ((constructor (150))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "aes/e2e");
}
