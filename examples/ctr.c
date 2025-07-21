#include <cryptone/aes.h>

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

ssize_t
fhexdump (FILE *stream, uint8_t const *buf, size_t buflen)
{
  ssize_t charn = 0;

  for (size_t i = 0; i < buflen; ++i)
    charn += fprintf (stream, "%.2x", buf[i]);
  charn += fprintf (stream, "\n");

  return charn;
}

ssize_t
hexdump (uint8_t const *buf, size_t buflen)
{
  return fhexdump (stdout, buf, buflen);
}

ssize_t
fhexdump_labeled (FILE *stream, const char *label, size_t label_max,
                  const uint8_t *buf, size_t buflen)
{
  char fmt[256];
  snprintf (fmt, sizeof (fmt) / sizeof (fmt[0]), "%%-%zus: ", label_max);

  ssize_t charn  = fprintf (stream, fmt, label);
  charn         += fhexdump (stream, buf, buflen);
  return charn;
}

ssize_t
hexdump_labeled (const char *label, size_t label_max, const uint8_t *buf,
                 size_t buflen)
{
  return fhexdump_labeled (stdout, label, label_max, buf, buflen);
}

uint8_t
randb (uint8_t min, uint8_t max)
{
  if (min > max)
  {
    uint8_t temp = min;
    min          = max;
    max          = temp;
  }
  return (uint8_t)(min + rand () % (max - min + 1));
}

void
randfill (uint8_t *buf, size_t len)
{
  int fd = open ("/dev/urandom", O_RDONLY);
  if (fd < 0)
    return;
  read (fd, buf, len);
  close (fd);
}

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