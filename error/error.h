#ifndef ERROR_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__ ((noreturn)) void _femiterror (FILE *stream, const char *s);
__attribute__ ((noreturn)) void vferrorf (FILE *stream, const char *fmt,
                                          va_list va);
__attribute__ ((noreturn)) void ferrorf (FILE *stream, const char *fmt,
                                         ...);
__attribute__ ((noreturn)) void vferroratf (FILE *stream, const char *at,
                                            const char *fmt, va_list va);
__attribute__ ((noreturn)) void ferroratf (FILE *stream, const char *at,
                                           const char *fmt, ...);

#endif