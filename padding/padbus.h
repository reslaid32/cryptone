#ifndef __PADBUS_IMP_H
#define __PADBUS_IMP_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t PAD_Identifier;

typedef enum PAD_Action
{
  PA_PAD,
  PA_UNPAD,
} PAD_Action;

typedef size_t (*PAD_Padder_bound_fn) (const uint8_t *src, size_t len,
                                       size_t     block_size,
                                       PAD_Action action);
typedef void (*PAD_Padder_process_fn) (uint8_t *dst, const uint8_t *src,
                                       size_t len, size_t block_size,
                                       PAD_Action action);

typedef struct PAD_Padder
{
  PAD_Padder_bound_fn   bound;
  PAD_Padder_process_fn process;
} PAD_Padder;

typedef struct PAD_Entry
{
  const char *name;
  PAD_Padder  padder;
} PAD_Entry;

int         PAD_CanRegister ();
size_t      PAD_MaxPadders ();
size_t      PAD_PaddersCount ();
size_t      PAD_PaddersLeftCount ();
int         PAD_Register (PAD_Entry padder, PAD_Identifier *out_id);
void        PAD_Dequeue (void);
int         PAD_Find (const char *name, PAD_Identifier *out_id);
void        PAD_Unregister (PAD_Identifier id);

PAD_Padder *PAD_Get (PAD_Identifier id);
PAD_Padder *PAD_GetByName (const char *name);

PAD_Entry  *PAD_GetEntryById (PAD_Identifier id);
PAD_Entry  *PAD_GetEntryByName (const char *name);

#endif
