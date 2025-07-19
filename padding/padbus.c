

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "../queue/aqueue.h"
#include "padbus.h"

#ifndef PADBUS_MAX_PADDERS
#define PADBUS_MAX_PADDERS 16
#endif

typedef struct PAD_EntryPair
{
  int            used;
  PAD_Identifier id;
  PAD_Entry      entry;
} PAD_EntryPair;

typedef struct PAD_Pending
{
  PAD_Entry padder;
} PAD_Pending;

static PAD_EntryPair g_padders[PADBUS_MAX_PADDERS] = { 0 };
static AtomicQueue   g_pending_padders;

__attribute__ ((constructor (102))) static void
PAD_Init ()
{
  AQ_Init (&g_pending_padders);
}

__attribute__ ((destructor)) static void
PAD_Shutdown ()
{
  AQ_Destroy (&g_pending_padders);
}

int
PAD_CanRegister ()
{
  return PAD_PaddersLeftCount () > 0;
}

size_t
PAD_MaxPadders ()
{
  return PADBUS_MAX_PADDERS;
}

size_t
PAD_PaddersCount ()
{
  size_t count = 0;
  for (size_t i = 0; i < PADBUS_MAX_PADDERS; ++i)
    if (g_padders[i].used)
      ++count;
  return count;
}

size_t
PAD_PaddersLeftCount ()
{
  size_t count = 0;
  for (size_t i = 0; i < PADBUS_MAX_PADDERS; ++i)
    if (!g_padders[i].used)
      ++count;
  return count;
}

static int
PAD_RegisterPadder (PAD_Entry padder, PAD_Identifier *out_id)
{
  for (PAD_Identifier i = 0; i < PADBUS_MAX_PADDERS; ++i)
  {
    if (g_padders[i].entry.name == NULL)
    {
      g_padders[i].used  = 1;
      g_padders[i].id    = i;
      g_padders[i].entry = padder;
      if (out_id)
        *out_id = i;
      return 0;
    }
  }
  return -1;
}

int
PAD_Register (PAD_Entry padder, PAD_Identifier *out_id)
{
  if (out_id)
    return PAD_RegisterPadder (padder, out_id);

  PAD_Pending *pending = malloc (sizeof (PAD_Pending));
  if (!pending)
    return -1;

  pending->padder = padder;
  AQ_Enqueue (&g_pending_padders, pending);

  return 0;
}

__attribute__ ((constructor (32767))) void
PAD_Dequeue (void)
{
  void *ptr;
  while ((ptr = AQ_Dequeue (&g_pending_padders)) != NULL)
  {
    PAD_Pending *pending = ptr;
    PAD_Entry    padder  = pending->padder;

    int          c;
    if ((c = PAD_RegisterPadder (padder, NULL)) != 0)
    {
      fprintf (stderr,
               "[padbus] PAD_Dequeue failed: PAD_RegisterPadder() "
               "returned %d\n",
               c);
    }
    free (pending);
  }
}

int
PAD_Find (const char *name, PAD_Identifier *out_id)
{
  if (name == NULL || out_id == NULL)
    return -1;

  for (PAD_Identifier i = 0; i < PADBUS_MAX_PADDERS; ++i)
  {
    if (g_padders[i].used && g_padders[i].entry.name != NULL)
    {
      if (strcmp (g_padders[i].entry.name, name) == 0)
      {
        *out_id = g_padders[i].id;
        return 0;
      }
    }
  }
  return -1;
}

void
PAD_Unregister (PAD_Identifier id)
{
  if (id < PADBUS_MAX_PADDERS)
    memset (&g_padders[id], 0, sizeof (PAD_EntryPair));
}

PAD_Padder *
PAD_Get (PAD_Identifier id)
{
  if (id < PADBUS_MAX_PADDERS && g_padders[id].used)
    return &g_padders[id].entry.padder;
  return NULL;
}

PAD_Padder *
PAD_GetByName (const char *name)
{
  if (name == NULL)
    return NULL;

  for (PAD_Identifier i = 0; i < PADBUS_MAX_PADDERS; ++i)
  {
    if (g_padders[i].used && g_padders[i].entry.name != NULL)
    {
      if (strcmp (g_padders[i].entry.name, name) == 0)
        return &g_padders[i].entry.padder;
    }
  }

  return NULL;
}

PAD_Entry *
PAD_GetEntryById (PAD_Identifier id)
{
  if (id < PADBUS_MAX_PADDERS && g_padders[id].used)
    return &g_padders[id].entry;
  return NULL;
}

PAD_Entry *
PAD_GetEntryByName (const char *name)
{
  if (name == NULL)
    return NULL;

  for (PAD_Identifier i = 0; i < PADBUS_MAX_PADDERS; ++i)
  {
    if (g_padders[i].used && g_padders[i].entry.name != NULL)
    {
      if (strcmp (g_padders[i].entry.name, name) == 0)
        return &g_padders[i].entry;
    }
  }
  return NULL;
}
