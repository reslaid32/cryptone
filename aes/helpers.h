#ifndef __AES_IMP_HELPERS_H
#define __AES_IMP_HELPERS_H

#include <stdint.h>

static uint8_t
xtime (uint8_t x)
{
  return (x << 1) ^ ((x >> 7) * 0x1b);
}

static uint8_t
gmul (uint8_t x, uint8_t y)
{
  uint8_t r = 0;
  while (y)
  {
    if (y & 1)
      r ^= x;
    x   = xtime (x);
    y >>= 1;
  }
  return r;
}

#endif