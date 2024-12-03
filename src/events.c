#include "settings.h"
#include "select.h"
#include "entry.h"

#include <sys/param.h>
#include <ncurses.h>

t_app_status process_event(t_entry *entries, int entry_count, int column_count, int *cursor)
{
  int key = getch();
  int x = *cursor % column_count;
  int y = *cursor / column_count;

  switch (key)
  {
    case 'q':
    case 33:
      return CANCLED;

    case '\n':
    case KEY_ENTER:
      if (g_settings.single_selection_flag)
        entries[*cursor].selected = true;
      return VALIDATED;

    case ' ':
    {
      if (!g_settings.single_selection_flag)
        entries[*cursor].selected = !entries[*cursor].selected;
      break;
    }

    case KEY_UP:
    {
      *cursor = MAX(0, x + (y - 1) * column_count);
      break;
    }
    case KEY_DOWN:
    {
      *cursor = MIN(entry_count - 1, x + (y + 1) * column_count);
      break;
    }
    case KEY_LEFT:
    {
      *cursor = ((x + column_count - 1) % column_count) + y * column_count;
      break;
    }
    case KEY_RIGHT:
    {
      *cursor = (x + 1) % column_count + y * column_count;
      break;
    }
  }
  return WAITING;
}
