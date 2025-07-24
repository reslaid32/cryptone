#include <string.h>

#include "../../include/cryptone/aes.h"
#include "../../unit/unit.h"

#define ARR_EQ(a, b, len)                                                 \
  UT_Assert (memcmp ((a), (b), (len)) == 0, "Arrays must be equal")

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
  test_encrypt_decrypt_block ();
}

__attribute__ ((constructor (200))) static void
_unit_registrar (void)
{
  UT_Register (_unit, "aes/unit");
}
