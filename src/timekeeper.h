/*
Copyright © 2020 Patrick Günthard <me@patrickguenthard.ch>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/
#ifndef TIMEKEEPER_H
#define TIMEKEEPER_H

#define NO_ERROR 0
#define ARGS_ERROR 1
#define ENTRY_ERROR 2
#define IO_ERROR 3
#define UNKN_ERROR 4

#define DATE_TIME_FORMAT "%Y:%m:%d %H:%M:%S";

#include <time.h>

struct tk_entry {
  time_t start;
  time_t end;
  char comment[256];
  
};

int parse_tk_entries(char *, struct tk_entry **);
void store_tk_entries(struct tk_entry *, int);
void list_entries(struct tk_entry *, int);
char *datetime_to_string(time_t);
char *duration_to_string(time_t);

int start_entry(struct tk_entry **, int, char *);
void end_entry(struct tk_entry **, int, char *);



void usage();

#endif /* TIMEKEEPER_H */
