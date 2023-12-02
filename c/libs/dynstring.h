#ifndef DYNSTRING_H
#define DYNSTRING_H

#include <stdio.h>
#include <string.h>

#define INIT_BUF_SIZE 64
#define CHUNK_SIZE 1024

typedef struct string {
  char *buffer;
  int bsize, len;
} string_t;

// Memory management
int init_string(string_t *string);
int free_string(string_t *string);

// Content management
int append_string(string_t *string, const char *data, int data_len);
void reset_string(string_t *string);

int read_line_from_file(string_t *string, FILE *file);

#endif
