#include "./lib.h"
#include <assert.h>
#include <cryptone/aes.h>
#include <string.h>

int
main (void)
{
  size_t  label_max = 10;

  uint8_t key[AES256_KEY_BYTES];

  uint8_t orig_iv[AES_BLOCK_SIZE];
  uint8_t iv[AES_BLOCK_SIZE];

  uint8_t plaintext[64]
      = "CBC mode test: encrypting 64 bytes of message data!";
  uint8_t ciphertext[sizeof (plaintext)];
  uint8_t decrypted[sizeof (plaintext)];

  randfill (key, sizeof (key));
  randfill (orig_iv, sizeof (iv));

  hexdump_labeled ("Key", label_max, key, sizeof (key));
  hexdump_labeled ("IV", label_max, iv, sizeof (iv));
  hexdump_labeled ("Plaintext", label_max, plaintext, sizeof (plaintext));

  AES_Context *ctx = AES_Context_Create ();
  AES_Context_FlushInit (ctx, key, sizeof (key));

  memcpy (iv, orig_iv, AES_BLOCK_SIZE);
  AES_Context_CBC_Encrypt (ctx, plaintext, ciphertext, sizeof (plaintext),
                           iv);

  hexdump_labeled ("Ciphertext", label_max, ciphertext,
                   sizeof (ciphertext));

  memcpy (iv, orig_iv, AES_BLOCK_SIZE);
  AES_Context_CBC_Decrypt (ctx, ciphertext, decrypted, sizeof (ciphertext),
                           iv);

  hexdump_labeled ("Decrypted", label_max, decrypted, sizeof (decrypted));

  assert (memcmp (plaintext, decrypted, sizeof (plaintext)) == 0);

  AES_Context_Destroy (ctx);

  return 0;
}
