/*
Copyright © 2020 Patrick Günthard <me@patrickguenthard.ch>
This work is free. You can redistribute it and/or modify it under the
terms of the Do What The Fuck You Want To Public License, Version 2,
as published by Sam Hocevar. See the COPYING file for more details.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "timekeeper.h"

char *history_file;

int main(int argc, char **argv)
{
  if(argc <= 1) {
    usage();
    exit(ARGS_ERROR);
  }

  history_file = getenv("HOME");
  strcat(history_file, "/.tkhistory");

  /* check if history file exists. If not, create it */
  FILE *test_file = fopen(history_file, "r");
  if (!test_file) {
    test_file = fopen(history_file, "w");
    printf("Create new history file %s\n", history_file);
    if(test_file) {
      fclose(test_file);
    }
    else {
      fprintf(stderr,"Unable to create history file\n");
      exit(IO_ERROR);
    }
  }
  
  struct tk_entry *entries = malloc(0);
  FILE *f = fopen(history_file,"r");
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
  else if(strcmp(argv[1], "stop") == 0) {
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
  else if(strcmp(argv[1], "status") == 0) {
    entry_status(entries, entries_count);
  }


  free(entries);
  return NO_ERROR;
}


int parse_tk_entries(char *string, struct tk_entry **entries)
{
  char *token, *tofree_l, *tofree_v;
  int entries_count = 0;
  tofree_l = strdup(string);

  for(token = strtok(string, "\n");
      token;
      token = strtok(NULL, "\n")) {
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
  FILE *f = fopen(history_file, "w");
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
    if(entries[i].end != 0) {
      printf("(%03x) S: %s   E: %s   D: %s (%s)\n", i, datetime_to_string(entries[i].start, false), datetime_to_string(entries[i].end, false), duration_to_string(entries[i].end - entries[i].start), entries[i].comment);
    }
    else {
      printf("(%03x) S: %s   Current             D: %s (%s)\n", i, datetime_to_string(entries[i].start, false), duration_to_string(time(NULL) - entries[i].start), entries[i].comment);
    }
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

void entry_status(struct tk_entry *entries, int entries_count)
{
  if (entries_count == 0 || entries[entries_count - 1].end != 0) {
    fprintf(stderr, "You have to start a new entry, before display the status\n");
    exit(ENTRY_ERROR);
  }
  printf("(%03x) S: %s   D: (%s)   (%s)\n", entries_count - 1, datetime_to_string(entries[entries_count-1].start, false), duration_to_string(time(NULL) - entries[entries_count-1].start), entries[entries_count - 1].comment);

  
}

char *datetime_to_string(time_t time, bool with_year)
{
  char *buffer = malloc(19 * sizeof(char));
  struct tm *tm_time = localtime(&time);
  if(with_year) {
    strftime(buffer, 19, "%Y-%m-%d %H:%M:%S", tm_time);
  }
  else {
    strftime(buffer, 19, "%m-%d %H:%M:%S", tm_time);
  }
  return buffer;
}

/* can't use strftime because UNIX epoche starts on 01:00:00 and not 00:00:00 */
char *duration_to_string(time_t secs)
{
  char *buffer = malloc(9 * sizeof(char));
  sprintf(buffer, "%02ld:%02ld:%02ld", secs / 3600, (secs / 60) % 60, secs % 60);
  return buffer;
}


void usage()
{
  /* to be implemented */
}
