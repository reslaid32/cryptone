#ifndef __X923_IMP_H
#define __X923_IMP_H

#include "padbus.h"

size_t PAD_X923_Bound (const uint8_t *src, size_t len, size_t block_size,
                       PAD_Action action);
void   PAD_X923_Process (uint8_t *dst, const uint8_t *src, size_t len,
                         size_t block_size, PAD_Action action);
int    PAD_X923_Get (PAD_Padder **pad);

#endif