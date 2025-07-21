#ifndef _LIB_H
#define _LIB_H

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static ssize_t
fhexdump (FILE *stream, uint8_t const *buf, size_t buflen)
{
  ssize_t charn = 0;

  for (size_t i = 0; i < buflen; ++i)
    charn += fprintf (stream, "%.2x", buf[i]);
  charn += fprintf (stream, "\n");

  return charn;
}

static ssize_t
hexdump (uint8_t const *buf, size_t buflen)
{
  return fhexdump (stdout, buf, buflen);
}

static ssize_t
fhexdump_labeled (FILE *stream, const char *label, size_t label_max,
                  const uint8_t *buf, size_t buflen)
{
  char fmt[256];
  snprintf (fmt, sizeof (fmt) / sizeof (fmt[0]), "%%-%zus: ", label_max);

  ssize_t charn  = fprintf (stream, fmt, label);
  charn         += fhexdump (stream, buf, buflen);
  return charn;
}

static ssize_t
hexdump_labeled (const char *label, size_t label_max, const uint8_t *buf,
                 size_t buflen)
{
  return fhexdump_labeled (stdout, label, label_max, buf, buflen);
}

static uint8_t
randb (uint8_t min, uint8_t max)
{
  if (min > max)
  {
    uint8_t temp = min;
    min          = max;
    max          = temp;
  }
  return (uint8_t)(min + rand () % (max - min + 1));
}

static void
randfill (uint8_t *buf, size_t len)
{
  int fd = open ("/dev/urandom", O_RDONLY);
  if (fd < 0)
    return;
  read (fd, buf, len);
  close (fd);
}

#endif