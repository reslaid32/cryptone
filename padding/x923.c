#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "padbus.h"

static const char *x923_name = "x923";

size_t
PAD_X923_Bound (const uint8_t *src, size_t len, size_t block_size,
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

    for (size_t i = 1; i < pad_val; i++)
      if (src[len - 1 - i] != 0)
        return len;
    return len - pad_val;
  }
}

void
PAD_X923_Process (uint8_t *dst, const uint8_t *src, size_t len,
                  size_t block_size, PAD_Action action)
{
  if (action == PA_PAD)
  {
    size_t rem     = len % block_size;
    size_t pad_len = block_size - rem;
    memcpy (dst, src, len);
    memset (dst + len, 0, pad_len - 1);
    dst[len + pad_len - 1] = (uint8_t)pad_len;
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

    for (size_t i = 1; i < pad_val; i++)
    {
      if (src[len - 1 - i] != 0)
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
  PAD_Entry ANSI_X923_PadderEntry = {
    .name = x923_name,
    .padder = {
      .bound = PAD_X923_Bound,
      .process = PAD_X923_Process,
    }
  };

  PAD_Register (ANSI_X923_PadderEntry, NULL);
}

int
PAD_X923_Get (PAD_Padder **pad)
{
  PAD_Identifier id;
  if (PAD_Find (x923_name, &id) != 0)
    return -1;

  *pad = PAD_Get (id);
  if (*pad == NULL)
    return -1;

  return 0;
}
