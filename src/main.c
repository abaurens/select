#include "entry.h"
#include "select.h"
#include "settings.h"

#include <stdio.h>

t_settings g_settings;

static void print_selection(t_entry *entries, int entry_count)
{
  int i;
  int selected_count;

  selected_count = 0;
  for (i = 0; i < entry_count; ++i)
  {
    if (!entries[i].selected)
      continue;
    if (selected_count++ > 0)
      printf("%s", g_settings.separator);
    printf("%s", entries[i].value);
  }
  printf("\n");
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

  if (present(entries, entry_count) == CANCLED)
    return EX_CANCLED;
  print_selection(entries, entry_count);
  return EX_SUCCESS;
}
