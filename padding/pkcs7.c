#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "padbus.h"

static const char *pkcs7_name = "pkcs7";

size_t
PAD_PKCS7_Bound (const uint8_t *src, size_t len, size_t block_size,
                 PAD_Action action)
{
  if (action == PA_PAD)
  {
    size_t rem     = len % block_size;
    size_t pad_len = block_size - rem;
    return len + pad_len;
  }
  else
  {
    if (len == 0)
      return 0;
    uint8_t pad_val = src[len - 1];
    if (pad_val == 0 || pad_val > block_size)
      return len;

    for (size_t i = 0; i < pad_val; i++)
      if (src[len - 1 - i] != pad_val)
        return len;
    return len - pad_val;
  }
}

void
PAD_PKCS7_Process (uint8_t *dst, const uint8_t *src, size_t len,
                   size_t block_size, PAD_Action action)
{
  if (action == PA_PAD)
  {
    size_t rem     = len % block_size;
    size_t pad_len = block_size - rem;
    memcpy (dst, src, len);
    memset (dst + len, (uint8_t)pad_len, pad_len);
  }
  else
  {
    if (len == 0)
      return;

    uint8_t pad_val = src[len - 1];
    if (pad_val == 0 || pad_val > block_size)
    {
      memcpy (dst, src, len);
      return;
    }

    for (size_t i = 0; i < pad_val; i++)
    {
      if (src[len - 1 - i] != pad_val)
      {
        memcpy (dst, src, len);
        return;
      }
    }

    size_t new_len = len - pad_val;
    memcpy (dst, src, new_len);
  }
}

__attribute__ ((constructor (105))) static void
_register ()
{
  PAD_Entry PKCS7_PadderEntry = {
    .name = pkcs7_name,
    .padder = {
        .bound = PAD_PKCS7_Bound,
        .process = PAD_PKCS7_Process,
    }};

  PAD_Register (PKCS7_PadderEntry, NULL);
}

int
PAD_PKCS7_Get (PAD_Padder **pad)
{
  PAD_Identifier id;
  if (PAD_Find (pkcs7_name, &id) != 0)
    return -1;

  *pad = PAD_Get (id);
  if (*pad == NULL)
    return -1;

  return 0;
}
