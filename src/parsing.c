#include "select.h"
#include "settings.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <limits.h>
#include <string.h>
#include <strings.h>

static const struct option g_long_options[] = {
  /* Flags */
  { "single",        no_argument, &g_settings.single_selection_flag, 1 },
  { "high-contrast", no_argument, &g_settings.high_contrast_flag,    1 },

  /* Options */
  { "delimiter", required_argument, 0, 'd' },
  { "column",    required_argument, 0, 'c' },
  { "help",      no_argument,       0, 'h' }
};


static void print_usage(const char *const app_name)
{
  printf("Usage: %s [OPTION]... [ENTRY]...\n", app_name);
  printf("  OPTIONS:\n");
  printf("   -h, --help:          Print this message and exit\n");
  printf("   -s, --single:        Enable single selection mode.\n");
  printf("   -d, --delimiter=sep: Specify how the selection should be separatedin the final result. (default=" ")\n");
  printf("   -c, --column=column: Specify how many column the choice must be displayed on. (default=auto)\n");
  printf("       --high-contrast: Enables high contrast mode.\n");
  printf("  ENTRIES: One or more entries to select from.\n");
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

int parse_arguments(int ac, char **av)
{
  char c;

  bzero(&g_settings, sizeof(t_settings));
  g_settings.separator = " ";
  while (true)
  {
    int option_index = 0;

    c = getopt_long(ac, av, "sc:d:h", g_long_options, &option_index);

    if (c == -1)
      break;

    switch (c)
    {
      case 0:
        break;

      case 'h':
        print_usage(*av);
        break;

      case 's':
        g_settings.single_selection_flag = true;
        break;

      case 'd':
        g_settings.separator = optarg;
        break;

      case 'c':
        parse_column_count(optarg);
        break;

      case '?':
        print_usage(*av);
        break;

      default:
        abort();
    }
  }

  return optind;
}
