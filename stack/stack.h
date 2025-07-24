#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct
{
  void  *data;
  int    top;
  int    capacity;
  size_t elem_size;
} Stack;

void Stack_Init (Stack *s, size_t elem_size, int capacity);
void Stack_Destroy (Stack *s);

int  Stack_Push (Stack *s, const void *value);
int  Stack_Pop (Stack *s, void *out);
int  Stack_Peek (Stack *s, void *out);

int  Stack_Empty (const Stack *s);
int  Stack_Full (const Stack *s);
int  Stack_Size (const Stack *s);

#endif // STACK_H
