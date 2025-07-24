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

void C1_Stack_Init (Stack *s, size_t elem_size, int capacity);
void C1_Stack_Destroy (Stack *s);

int  C1_Stack_Push (Stack *s, const void *value);
int  C1_Stack_Pop (Stack *s, void *out);
int  C1_Stack_Peek (Stack *s, void *out);

int  C1_Stack_Empty (const Stack *s);
int  C1_Stack_Full (const Stack *s);
int  C1_Stack_Size (const Stack *s);

#endif // STACK_H
