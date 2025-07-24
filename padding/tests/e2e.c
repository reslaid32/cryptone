#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/cryptone/padding/padbus.h"
#include "../../log/log.h"
#include "../../unit/unit.h"

void
test_padder (PAD_Identifier id, const char *name, PAD_Padder *padder,
             uint8_t *orig_buf, size_t orig_len, size_t block_size)
{
  (void)id;
  (void)name;

  if (!padder || !orig_buf)
    return;

  C1_LOG_Verbose ("\tCurrent padder: \t{ .id = %u, .name = \"%s\" }\n", id,
                  name);

  size_t padded_len
      = padder->bound (orig_buf, orig_len, block_size, PA_PAD);

  uint8_t  fixed_padded_buf[256];
  uint8_t  fixed_unpadded_buf[256];

  uint8_t *padded_buf         = fixed_padded_buf;
  uint8_t *unpadded_buf       = fixed_unpadded_buf;

  int      need_free_padded   = 0;
  int      need_free_unpadded = 0;

  if (padded_len > sizeof (fixed_padded_buf))
  {
    padded_buf = (uint8_t *)malloc (padded_len);
    if (!padded_buf)
    {
      C1_LOG_Fatal ("[  padded_buf] \tAllocation error\n");
      return;
    }
    need_free_padded = 1;
  }

  padder->process (padded_buf, orig_buf, orig_len, block_size, PA_PAD);

  C1_LOG_Verbose ("\tOriginal length:\t%zu\n", orig_len);
  C1_LOG_Verbose ("\tPadded length:\t\t%zu\n", padded_len);

  C1_LOG_Verbose ("\tPadded data:\t\t");
  for (size_t i = 0; i < padded_len; i++)
    C1_LOG_printf ("%02X ", padded_buf[i]);
  C1_LOG_printf ("\n");

  size_t unpadded_len
      = padder->bound (padded_buf, padded_len, block_size, PA_UNPAD);

  if (unpadded_len > sizeof (fixed_unpadded_buf))
  {
    unpadded_buf = (uint8_t *)malloc (unpadded_len);
    if (!unpadded_buf)
    {
      C1_LOG_Fatal ("[unpadded_buf] \tAllocation error\n");
      if (need_free_padded)
        free (padded_buf);
      return;
    }
    need_free_unpadded = 1;
  }

  padder->process (unpadded_buf, padded_buf, padded_len, block_size,
                   PA_UNPAD);

  C1_LOG_Verbose ("\tUnpadded length:\t%zu\n", unpadded_len);
  C1_LOG_Verbose ("\tUnpadded data:\t\t");
  for (size_t i = 0; i < unpadded_len; i++)
    C1_LOG_printf ("%02X ", unpadded_buf[i]);
  C1_LOG_printf ("\n");

  UT_Assert (memcmp (orig_buf, unpadded_buf, orig_len) == 0,
             "unpadded_buf must be eq orig_buf");

  if (need_free_padded)
    free (padded_buf);
  if (need_free_unpadded)
    free (unpadded_buf);
}

int
test_padder_wrapper (const char *name, uint8_t *buf, size_t len,
                     size_t block_size)
{
  // finds id by name (on registered padders)
  PAD_Identifier id;
  if (PAD_Find (name, &id) != 0)
    return -1;

  // gets padder by id
  PAD_Padder *padder;
  if ((padder = PAD_Get (id)) == NULL)
    return -2;

  // tests padding with this padder
  test_padder (id, name, padder, buf, len, block_size);
  return 0;
}

int test_padder_wrapper (const char *name, uint8_t *buf, size_t len,
                         size_t block_size);

void
_unit ()
{
  C1_LOG_PushFrame ();
  C1_LOG_SetAutoNewline (0);

  uint8_t input[20]  = { 1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                         11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
  size_t  input_len  = 20;
  size_t  block_size = 8;

  test_padder_wrapper ("pkcs7", input, input_len, block_size);
  test_padder_wrapper ("x923", input, input_len, block_size);

  C1_LOG_PopFrame ();
}

__attribute__ ((constructor (200))) void
_unit_registrar ()
{
  UT_Register (_unit, "padding/unit");
}
