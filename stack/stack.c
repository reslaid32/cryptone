#include "stack.h"
#include <stdlib.h>
#include <string.h>

void
C1_Stack_Init (Stack *s, size_t elem_size, int capacity)
{
  s->data = malloc (elem_size * capacity);
  if (!s->data)
  {
    free (s);
    return;
  }

  s->top       = -1;
  s->capacity  = capacity;
  s->elem_size = elem_size;
}

void
C1_Stack_Destroy (Stack *s)
{
  if (s)
    free (s->data);
}

int
C1_Stack_Push (Stack *s, const void *value)
{
  if (C1_Stack_Full (s))
    return 0;

  s->top++;
  void *dest = (char *)s->data + s->top * s->elem_size;
  memcpy (dest, value, s->elem_size);
  return 1;
}

int
C1_Stack_Pop (Stack *s, void *out)
{
  if (C1_Stack_Empty (s))
    return 0;

  void *src = (char *)s->data + s->top * s->elem_size;
  if (out)
    memcpy (out, src, s->elem_size);
  s->top--;
  return 1;
}

int
C1_Stack_Peek (Stack *s, void *out)
{
  if (C1_Stack_Empty (s))
    return 0;

  void *src = (char *)s->data + s->top * s->elem_size;
  if (out)
    memcpy (out, src, s->elem_size);
  return 1;
}

int
C1_Stack_Empty (const Stack *s)
{
  return s->top == -1;
}

int
C1_Stack_Full (const Stack *s)
{
  return s->top >= s->capacity - 1;
}

int
C1_Stack_Size (const Stack *s)
{
  return s->top + 1;
}
