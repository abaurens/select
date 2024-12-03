#ifndef ENTRY_H_
#define ENTRY_H_

#include <stdbool.h>

typedef struct s_entry t_entry;
struct s_entry
{
  bool selected;
  const char *value;
};


#endif
