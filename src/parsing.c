#include "select.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>

#define WRITE_LSTR(_str) void_write(1, _str, sizeof(_str) - 1)

#define SHORT_OPTIONS "hvsn1p:c:d:"

/* We need this to warn about conflicting flags */
static int g_skip_single_choice_flag = 0;
static int g_no_skip_single_choice_flag = 0;

static const struct option g_long_options[] = {
  /* Flags */
  { "single",         no_argument, &g_settings.single_selection_flag, 1 },
  { "skip-single",    no_argument, &g_skip_single_choice_flag,        1 },
  { "no-skip-single", no_argument, &g_no_skip_single_choice_flag,     1 },
  { "high-contrast",  no_argument, &g_settings.high_contrast_flag,    1 },

  /* Options */
  { "prompt",    required_argument, 0, 'p' },
  { "delimiter", required_argument, 0, 'd' },
  { "column",    required_argument, 0, 'c' },
  { "version",   no_argument,       0, 'v' },
  { "help",      no_argument,       0, 'h' },
  {0, 0, 0, 0}
};

const char g_escapable[][2] = {
  { '0',  '\0' },
  { 'a',  '\a' },
  { 'b',  '\b' },
  { 't',  '\t' },
  { 'n',  '\n' },
  { 'v',  '\v' },
  { 'f',  '\f' },
  { 'r',  '\r' },
  { 0, 0 }
};

static void print_usage(const char *const app_name)
{
  WRITE_LSTR("Usage: ");
  print(1, app_name);
  WRITE_LSTR(" [OPTION]... [ENTRY]...\n");

  WRITE_LSTR("  OPTIONS:\n");
  WRITE_LSTR("   -h, --help:           Print this message and exit.\n");
  WRITE_LSTR("   -v, --version:        Show the version number and exit.\n");
  WRITE_LSTR("   -s, --single:         Enable single selection mode.\n");
  WRITE_LSTR("   -1, --skip-single:    Skip user input whenever there is only one choice available. (default with --single)\n");
  WRITE_LSTR("   -n, --no-skip-single: Never skip user input. (default without --single)\n");
  WRITE_LSTR("   -p, --prompt=prompt:  Displays the provided text above the selection.\n");
  WRITE_LSTR("   -d, --delimiter=sep:  Specify how the selection should be separated in the final result. (default=\" \")\n");
  WRITE_LSTR("   -c, --column=column:  Specify how many column the choice must be displayed on. (default=auto)\n");
  WRITE_LSTR("       --high-contrast:  Enables high contrast mode.\n");
  WRITE_LSTR("  ENTRIES: One or more entries to select from.\n");
  exit(EX_SUCCESS);
}

static bool parse_integer(const char *const value_str, int *dest)
{
  int     i;
  int     len;
  int64_t val;

  len = strlen(value_str);
  if (len > 11)
    return false;

  i = (*value_str == '-' || *value_str == '+');

  while (value_str[i])
  {
    if (value_str[i] < '0' || value_str[i] > '9')
      return false;
    ++i;
  }

  val = atoll(value_str);
  if (val < INT_MIN || val > INT_MAX)
    return false;

  *dest = (int)val;
  return true;
}

static void parse_column_count(const char *const value)
{
  if (strcmp(value, "auto") == 0)
    return;

  if (!parse_integer(value, &g_settings.column_count) || g_settings.column_count < 0)
    fatal_error(EX_PARSING_ERROR, "Invalid argument for column option -- '%s'", value);
}

/*
** C11 5.1.2.2.1/2 says:
** "The parameters `argc` and `argv` and the strings pointed to by
** the `argv` array shall be modifiable by the program, and retain their
** last-stored values between program startup and program termination."
**
** This means we can safely evaluate escaped characters in place, as this
** process only reduces the total string length.
*/
char *parse_delimiter(char *const delimiter)
{
  int i;
  char *pos = delimiter;
  char *end = pos + strlen(pos);

  while (pos && *(pos = strchrnul(pos, '\\')))
  {
    ++pos;
    for (i = 0; *g_escapable[i]; ++i)
    {
      if (*pos == 0)
        continue;

      if (*pos == g_escapable[i][0])
      {
        *pos = g_escapable[i][1];
        break;
      }
    }
    memmove(pos - 1, pos, end - pos);
    *--end = 0;
  }
  return delimiter;
}

int parse_arguments(int ac, char **av)
{
  char c;

  bzero(&g_settings, sizeof(t_settings));
  g_settings.separator = isatty(1) ? " " : "\n";

  while (true)
  {
    int option_index = 0;

    c = getopt_long(ac, av, SHORT_OPTIONS, g_long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 0:
        break;

      case 'h':
        print_usage(*av);
        break;

      case 'v':
        WRITE_LSTR(VERSION);
        exit(EX_SUCCESS);
        break;

      case 's':
        g_settings.single_selection_flag = true;
        break;

      case 'n':
        g_no_skip_single_choice_flag = true;
        break;

      case '1':
        g_skip_single_choice_flag = true;
        break;

      case 'c':
        parse_column_count(optarg);
        break;

      case 'd':
        g_settings.separator = parse_delimiter(optarg);
        break;

      case 'p':
        g_settings.prompt = optarg;
        break;

      case '?':
        print_usage(*av);
        break;

      default:
        abort();
    }
  }

  /* Ensure the single choice skiping flags aren't conflicting */
  if (g_skip_single_choice_flag && g_no_skip_single_choice_flag)
    fatal_error(EX_BAD_USAGE_ERROR, "Conflicting flags: --skip-single and --no-skip-single both present.");

  /* Then reflect them in the actual settings */
  if (g_skip_single_choice_flag || g_no_skip_single_choice_flag)
    g_settings.skip_single_choice = g_skip_single_choice_flag;
  else
    g_settings.skip_single_choice = g_settings.single_selection_flag;

  return optind;
}
