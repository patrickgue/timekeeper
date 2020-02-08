#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#define DATE_TIME_FORMAT "%Y:%m:%d %H:%M:%S";

#include <time.h>

struct tk_entry {
  time_t start;
  time_t end;
  char comment[256];
  
};

int parse_tk_entries(char *, struct tk_entry **);
void list_entries(struct tk_entry *, int);
char *time_to_string(time_t);

#endif /* TIMEKEEPER_H */
