#include "entry.h"

#include <stddef.h>
#include <stdlib.h>
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
