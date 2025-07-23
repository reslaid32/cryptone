#include "../include/cryptone/aes.h"

#include "../api/api.h"
#include "../error/error.h"

#include <stdlib.h>
#include <string.h>

WEAK void
AES_Context_EncryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  (void)ctx;
  (void)in;
  (void)out;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_DecryptBlock (const AES_Context *ctx, const uint8_t *in,
                          uint8_t *out)
{
  (void)ctx;
  (void)in;
  (void)out;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_ECB_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  (void)ctx;
  (void)in;
  (void)out;
  (void)len;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_ECB_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len)
{
  (void)ctx;
  (void)in;
  (void)out;
  (void)len;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_CBC_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  (void)ctx;
  (void)in;
  (void)out;
  (void)len;
  (void)iv;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_CBC_Decrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t iv[AES_BLOCK_SIZE])
{
  (void)ctx;
  (void)in;
  (void)out;
  (void)len;
  (void)iv;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}

WEAK void
AES_Context_CTR_Encrypt (const AES_Context *ctx, const uint8_t *in,
                         uint8_t *out, size_t len,
                         uint8_t nonce[AES_BLOCK_SIZE])
{
  (void)ctx;
  (void)in;
  (void)out;
  (void)len;
  (void)nonce;

  ferroratf (stderr, "/aes/waes.c", "unimplemented yet");
}
