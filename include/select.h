#ifndef SELECT_H_
#define SELECT_H_

#include "entry.h"

#include <stddef.h>

#ifndef VERSION
#define VERSION "<unknown-version>\n"
#endif

enum e_exit_codes
{
  EX_SUCCESS,
  EX_CANCLED,
  EX_ERROR,
  EX_PARSING_ERROR,
  EX_ALLOCATION_ERROR
};

typedef enum
{
  WAITING,
  VALIDATED,
  CANCLED
} t_app_status;


#define PRINTF_LIKE(_arg) __attribute__((format(printf, _arg, (_arg + 1))))

/* parsing.c */
int parse_arguments(int ac, char **av);

/* error.c */
void error(const char *const format, ...) PRINTF_LIKE(1);
void fatal_error(int exit_code, const char *const format, ...) PRINTF_LIKE(2);

/* memory.c */
t_entry *allocate_entries(size_t entry_count, char **entries);

/* UI.c */
t_app_status present(t_entry *entries, int entry_count);

/* events.c */
t_app_status process_event(t_entry *entries, int entry_count, int column_count, int *cursor);

#endif
