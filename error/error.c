#include "error.h"
#include <stdio.h>

__attribute__ ((noreturn)) void
_femiterror (FILE *stream, const char *s)
{
  fputs (s, stream);
  fputc ('\n', stream);
  abort ();
}

__attribute__ ((noreturn)) void
vferrorf (FILE *stream, const char *fmt, va_list va)
{
  const size_t buf_max = 512;
  char         buffer[buf_max];
  vsnprintf (buffer, buf_max, fmt, va);
  _femiterror (stream, buffer);
}

__attribute__ ((noreturn)) void
ferrorf (FILE *stream, const char *fmt, ...)
{
  va_list va;
  va_start (va, fmt);
  vferrorf (stream, fmt, va);
  va_end (va);
}

__attribute__ ((noreturn)) void
vferroratf (FILE *stream, const char *at, const char *fmt, va_list va)
{
  fprintf (stream, "%s: error: ", at);
  vferrorf (stream, fmt, va);
}

__attribute__ ((noreturn)) void
ferroratf (FILE *stream, const char *at, const char *fmt, ...)
{
  va_list va;
  va_start (va, fmt);
  vferroratf (stream, at, fmt, va);
  va_end (va);
}
