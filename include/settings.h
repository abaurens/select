#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct s_settings t_settings;
struct s_settings
{
  int single_selection_flag;
  int skip_single_choice;
  int high_contrast_flag;
  int column_count;
  const char *separator;
};

extern t_settings g_settings;

#endif
