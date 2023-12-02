#include "dynstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internal functions
int double_size(string_t *string) {
  char *tmp = (char *)realloc(string->buffer, string->bsize * 2 * sizeof(char));
  if (!tmp) {
    fprintf(stderr, "[%s:%d] Unable to reallocate memory\n", __FILE__,
            __LINE__);
    return 0;
  }

  string->buffer = tmp;
  string->bsize <<= 1;

  return 1;
}
// ===================

int init_string(string_t *string) {
  if (!string) {
    fprintf(stderr,
            "Pointer to string points to nothing. Unable to initialize...\n");
    return 0;
  }

  string->buffer = (char *)calloc(INIT_BUF_SIZE, sizeof(char));
  if (!string->buffer) {
    fprintf(stderr, "[%s:%d] Unable to allocate memory\n", __FILE__, __LINE__);
    return 0;
  }

  string->bsize = INIT_BUF_SIZE;
  string->len = 0;

  return 1;
}

int free_string(string_t *string) {
  if (!string) {
    fprintf(stderr, "Pointer to string points to nothing. Unable to free...\n");
    return 0;
  }

  if (string->buffer) {
    free(string->buffer);
    string->buffer = NULL;
    string->len = 0;
    string->bsize = 0;
  }

  return 1;
}

// Content management
int append_string(string_t *string, const char *data, int data_len) {
  char *endp = NULL;
  int init_len = string->len;

  while (data_len > (string->bsize - string->len - 1)) {
    if (!double_size(string)) {
      fprintf(stderr, "[%s:%d] Unable to reallocate memory\n", __FILE__,
              __LINE__);
      return -1;
    }
  }

  endp = stpncpy(string->buffer + string->len, data, data_len);
  *endp = '\0';

  string->len = strlen(string->buffer);

  return string->len - init_len;
}

void reset_string(string_t *string) {
  bzero(string->buffer, string->bsize);
  string->len = 0;
}

int read_line_from_file(string_t *string, FILE *file) {
  char tmp_buf[CHUNK_SIZE];
  int gcount = 0, lcount = 0;
  int new_line_index = 0;
  int keep_on_looping = 1;

  do {
    if (!fgets(tmp_buf, CHUNK_SIZE, file))
      return gcount;

    new_line_index = strcspn(tmp_buf, "\n");
    if (new_line_index < CHUNK_SIZE - 1) {
      keep_on_looping = 0;
      tmp_buf[new_line_index] = 0;
    }

    if ((lcount = append_string(string, tmp_buf, CHUNK_SIZE)) < 0) {
      fprintf(stderr, "[%s:%d] Unable to copy string: %s\n", __FILE__, __LINE__,
              tmp_buf);
      return -1;
    }
    gcount += lcount;
  } while (keep_on_looping);

  return gcount;
}
