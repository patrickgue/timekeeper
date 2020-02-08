/*
Copyright © 2020 Patrick Günthard <me@patrickguenthard.ch>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "timekeeper.h"

int main(int argc, char **argv)
{
  struct tk_entry *entries = malloc(0);
  FILE *f = fopen("./test/example.tkhistory","rw");
  int length;
  int nr_of_entries;
  char *buffer;
  if (f) {
    fseek (f, 0, SEEK_END);
    length = ftell (f);
    fseek (f, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer) {
      fread (buffer, 1, length, f);
    }
    fclose (f);
    nr_of_entries = parse_tk_entries(buffer, &entries);
    list_entries(entries, nr_of_entries);
  }
  else {
    fprintf(stderr, "Unable to read file\n");
  }

  return 0;
}


int parse_tk_entries(char *string, struct tk_entry **entries)
{
  char *token, *tofree_l, *tofree_v;
  int entries_count = 0;
  tofree_l = strdup(string);

  while ((token = strsep(&string, "\n")) != NULL) {
    if(strlen(token) > 0) {
      tofree_v = strdup(token);
      char   *start = strsep(&token, ",");
      char     *end = strsep(&token, ",");
      char *comment = strsep(&token, ",");
      free(tofree_v);
      entries_count++;
      *entries = realloc(*entries, sizeof(struct tk_entry) * entries_count);
      (*entries)[entries_count-1].start = atoi(start);
      (*entries)[entries_count-1].end = atoi(end);
      strcpy((*entries)[entries_count-1].comment, comment);
    }
  }

  free(tofree_l);

  return entries_count;
}


void list_entries(struct tk_entry *entries, int entries_count)
{
  int i;
  for(i = 0; i < entries_count; i++) {
    printf("(%2i) S: %s   E: %s  (%s)\n", i, time_to_string(entries[i].start), time_to_string(entries[i].end), entries[i].comment);
  }
}

char *time_to_string(time_t time)
{
  char *buffer = malloc(26 * sizeof(char));
  struct tm *tm_time = localtime(&time);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_time);
  return buffer;
}
