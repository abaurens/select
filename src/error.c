#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

static void error_impl(const char *const format, va_list ap)
{
  fprintf(stderr, "error: ");
  if (format)
    vfprintf(stderr, format, ap);
  else
    fprintf(stderr, "%s", strerror(errno));
  fprintf(stderr, "\n");
}

void error(const char *const format, ...)
{
  va_list ap;

  va_start(ap, format);
  error_impl(format, ap);
  va_end(ap);
}

void fatal_error(int exit_code, const char *const format, ...)
{
  va_list ap;

  va_start(ap, format);
  error_impl(format, ap);
  va_end(ap);
  exit(exit_code);
}
