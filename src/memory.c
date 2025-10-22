#include "entry.h"

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>

t_entry *allocate_entries(size_t entry_count, char **entries)
{
  size_t   i;
  t_entry *result;

  result = malloc(sizeof(t_entry) * entry_count);
  if (!result)
    return NULL;

  bzero(result, sizeof(t_entry) * entry_count);

  for (i = 0; i < entry_count; ++i)
    result[i].value = entries[i];

  return result;
}

void void_write(int fd, const char *const str, int size)
{
  (void)!write(fd, str, size);
}

void print(int fd, const char *const str)
{
  void_write(fd, str, strlen(str));
}
