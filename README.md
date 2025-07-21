## cryptone
#### Is a lightweight C library that provides AES cryptographic procedures with a clean API, including support for AES-128 and AES-256 in ECB, CBC, and CTR modes.
#### Support for other algorithms besides AES is also planned for the future.


## C-API Example (AES256-CTR):

```c
#include <cryptone/aes.h>

void    randfill        (uint8_t *buf, size_t len);
ssize_t hexdump_labeled (const char *label, size_t label_max, const uint8_t *buf, size_t buflen);

int
main (void)
{
  size_t  label_max = 10;

  uint8_t key[AES256_KEY_BYTES];
  uint8_t nonce[AES_BLOCK_SIZE];
  uint8_t plaintext[64]
      = "CTR mode test: encrypting 64 bytes of message data!";
  uint8_t ciphertext[sizeof (plaintext)];
  uint8_t decrypted[sizeof (plaintext)];

  randfill (key, sizeof (key));
  randfill (nonce, sizeof (nonce));

  hexdump_labeled ("Key", label_max, key, sizeof (key));
  hexdump_labeled ("Nonce", label_max, nonce, sizeof (nonce));
  hexdump_labeled ("Plaintext", label_max, plaintext, sizeof (plaintext));

  AES_Context *ctx = AES_Context_Create ();
  AES_Context_FlushInit (ctx, key, sizeof (key));

  AES_Context_CTR_Encrypt (ctx, plaintext, ciphertext, sizeof (plaintext),
                           nonce);
  hexdump_labeled ("Ciphertext", label_max, ciphertext,
                   sizeof (ciphertext));

  AES_Context_FlushInit (ctx, key, sizeof (key));
  AES_Context_CTR_Encrypt (ctx, ciphertext, decrypted, sizeof (ciphertext),
                           nonce);
  hexdump_labeled ("Decrypted", label_max, decrypted, sizeof (decrypted));

  AES_Context_Destroy (ctx);

  return 0;
}
```

