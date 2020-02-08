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
  if(argc <= 1) {
    usage();
    exit(ARGS_ERROR);
  }

  /* check if history file exists. If not, create it */
  FILE *test_file = fopen("./test/example.tkhistory", "r");
  if (!test_file) {
    test_file = fopen("./test/example.tkhistory", "w");
    if(test_file) {
      fclose(test_file);
    }
    else {
      fprintf(stderr,"Unable to create history file\n");
      exit(IO_ERROR);
    }
  }
  
  struct tk_entry *entries = malloc(0);
  FILE *f = fopen("./test/example.tkhistory","r");
  int length;
  int entries_count;
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
    entries_count = parse_tk_entries(buffer, &entries);
  }
  else {
    fprintf(stderr, "Unable to read file\n");
    exit(IO_ERROR);
  }

  if (strcmp(argv[1], "list") == 0) {
    list_entries(entries, entries_count);
  }
  else if(strcmp(argv[1], "start") == 0) {
    if(argc == 2) {
      entries_count = start_entry(&entries, entries_count, "");
    }
    else {
      entries_count = start_entry(&entries, entries_count, argv[2]);
    }
    store_tk_entries(entries, entries_count);
  }
  else if(strcmp(argv[1], "end") == 0) {
    if(argc == 2) {
      end_entry(&entries, entries_count, "");
    }
    else {
      end_entry(&entries, entries_count, argv[2]);
    }
    store_tk_entries(entries, entries_count);
  }
  else if(strcmp(argv[1], "new") == 0) {
    if(argc == 2) {
      end_entry(&entries, entries_count, "");
      entries_count = start_entry(&entries, entries_count, "");
    }
    else {
      end_entry(&entries, entries_count, "");
      entries_count = start_entry(&entries, entries_count, argv[2]);
    }
    store_tk_entries(entries, entries_count);
  }


  free(entries);
  return NO_ERROR;
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

void store_tk_entries(struct tk_entry *entries, int entries_count) {
  int i;
  FILE *f = fopen("./test/example.tkhistory", "w");
  if(f) {
    for(i = 0; i < entries_count; i++) {
      fprintf(f, "%ld,%ld,%s\n",entries[i].start, entries[i].end, entries[i].comment);
    }
    fclose(f);
  }
  else {
    fprintf(stderr, "Could not open file to store entries\n");
    exit(IO_ERROR);
  }
}

void list_entries(struct tk_entry *entries, int entries_count)
{
  int i;
  for(i = 0; i < entries_count; i++) {
    printf("(%2i) S: %s   E: %s  (%s)\n", i, datetime_to_string(entries[i].start), datetime_to_string(entries[i].end), entries[i].comment);
  }
}

int start_entry(struct tk_entry **entries, int entries_count, char *comment)
{
  if (entries_count > 0 && (*entries)[entries_count - 1].end == 0) {
    fprintf(stderr, "You have to end your current entry first, before you can start a new one\n");
    exit(ENTRY_ERROR);
  }
  entries_count++;
  (*entries) = realloc(*entries, sizeof(struct tk_entry) * entries_count);
  (*entries)[entries_count - 1].start = time(NULL);
  (*entries)[entries_count - 1].end = 0;
  strcpy((*entries)[entries_count - 1].comment, comment);
  
  return entries_count;
}

void end_entry(struct tk_entry **entries, int entries_count, char *comment)
{
  if (entries_count == 0 || (*entries)[entries_count - 1].end != 0) {
    fprintf(stderr, "You have to start a new entry, before you can end one\n");
    exit(ENTRY_ERROR);
  }

  (*entries)[entries_count - 1].end = time(NULL);
  if(strlen(comment) > 0) {
    strcpy((*entries)[entries_count - 1].comment, comment);
  }
}

char *datetime_to_string(time_t time)
{
  char *buffer = malloc(26 * sizeof(char));
  struct tm *tm_time = localtime(&time);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_time);
  return buffer;
}


void usage()
{
  // to be implemented
}
