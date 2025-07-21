#ifndef __CRYPTONE_PADDING_X923_H
#define __CRYPTONE_PADDING_X923_H

#include "padbus.h"

size_t PAD_X923_Bound (const uint8_t *src, size_t len, size_t block_size,
                       PAD_Action action);
void   PAD_X923_Process (uint8_t *dst, const uint8_t *src, size_t len,
                         size_t block_size, PAD_Action action);
int    PAD_X923_Get (PAD_Padder **pad);

#endif