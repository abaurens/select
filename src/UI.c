#include "settings.h"
#include "select.h"
#include "entry.h"

#include <stddef.h>
#include <string.h>
#include <sys/param.h>

#include <ncurses.h>

#define MARGIN 2

static int calc_max_column_count(const t_entry *entries, size_t entry_count)
{
  int len;
  int col_width;
  const t_entry *cur;
  const t_entry *end;

  col_width = 0;
  end = entries + entry_count;
  for (cur = entries; cur != end; ++cur)
  {
    len = strlen(cur->value);
    if (len > col_width)
      col_width = len;
  }

  if (!g_settings.single_selection_flag)
    col_width += 4; /* Add room for the checkbox "[x] " */
  else
    col_width += 2; /* Add room for the bracket "[...]" */
  col_width += MARGIN;

  return MAX(1, COLS / col_width);
}

static void present_entry(const t_entry *entry, int x, int y, bool hovered)
{
  const int hover_code = (g_settings.high_contrast_flag ? A_REVERSE : A_UNDERLINE);
  if (hovered)
    attron(hover_code);
  if (g_settings.single_selection_flag)
    mvprintw(y, x, "[%s]", entry->value);
  else
    mvprintw(y, x, "[%c] %s", entry->selected ? 'x' : ' ', entry->value);
  if (hovered)
    attroff(hover_code);
}

t_app_status present(t_entry *entries, int entry_count)
{
  int i;
  int x;
  int y;
  int start_height;
  int column_width;
  int column_count;
  int cursor;

  FILE *f = fopen("/dev/tty", "r+");
  SCREEN *screen = newterm(NULL, f, f);
  set_term(screen);

  noecho();
  keypad(stdscr, TRUE);

  start_height = 0;
  if (g_settings.prompt)
  {
    start_height = 1;
    for (i = 0; g_settings.prompt[i]; ++i)
      start_height += (g_settings.prompt[i] == '\n');
  }

  cursor = 0;
  do
  {
    clear();
    column_count = calc_max_column_count(entries, entry_count);
    if (g_settings.column_count > 0 && g_settings.column_count < column_count)
      column_count = g_settings.column_count;

    column_width = COLS / column_count;

    if (column_count > entry_count)
      column_count = entry_count;

    if (g_settings.prompt)
      mvprintw(0, 0, "%s", g_settings.prompt);

    for (i = 0; i < entry_count; ++i)
    {
      x = i % column_count;
      y = i / column_count;
      present_entry(entries + i, x * column_width, y + start_height,
        ((cursor % column_count) == x) && ((cursor / column_count) == y)
      );
    }
    refresh();
    i = process_event(entries, entry_count, column_count, &cursor);
  } while (i == WAITING);

  endwin();
  delscreen(screen);
  return i;
}
