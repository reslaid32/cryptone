#ifndef __PKCS7_IMP_H
#define __PKCS7_IMP_H

#include "padbus.h"

size_t PAD_PKCS7_Bound (const uint8_t *src, size_t len, size_t block_size,
                        PAD_Action action);
void   PAD_PKCS7_Process (uint8_t *dst, const uint8_t *src, size_t len,
                          size_t block_size, PAD_Action action);
int    PAD_PKCS7_Get (PAD_Padder **pad);

#endif