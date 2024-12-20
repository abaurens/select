#include "entry.h"
#include "select.h"
#include "settings.h"

#include <string.h>
#include <stdlib.h>

t_settings g_settings;

static void print_selection(t_entry *entries, int entry_count)
{
  int        i;
  int        sep_len;
  int        selected_count;
  const char *entry_str;

  selected_count = 0;
  sep_len = strlen(g_settings.separator);
  for (i = 0; i < entry_count; ++i)
  {
    if (!entries[i].selected)
      continue;
    entry_str = entries[i].value;
    if (selected_count++ > 0)
      void_write(1, g_settings.separator, sep_len);
    void_write(1, entry_str, strlen(entry_str));
  }
  void_write(1, "\n", 1);
}

int main(int ac, char **av)
{
  int opt_count;
  int entry_count;
  t_entry *entries;

  opt_count = parse_arguments(ac, av);
  entry_count = ac - opt_count;
  if (entry_count == 0)
    return EX_SUCCESS;

  entries = allocate_entries(entry_count, av + opt_count);
  if (!entries)
    fatal_error(EX_ALLOCATION_ERROR, NULL);

  if (present(entries, entry_count) == CANCELLED)
    return EX_CANCELLED;
  print_selection(entries, entry_count);
  free(entries);
  return EX_SUCCESS;
}
