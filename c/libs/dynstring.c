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

/* =========================================================================
 * Function: init_string
 *    Initialize a string structure with a buffer of size INIT_BUF_SIZE
 *
 * string_t *string: A pointer on the structure to initialize
 *
 * returns: 0 if an error occurs (pointer is NULL or mem allocation fail
 *          1 otherwise
 */
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

/* =========================================================================
 * Function: free_string
 *    Deallocate buffer memory inside a string structure
 *
 * string_t *string: a pointer on the structure to deallocate
 *
 * returns: 0 if string points to NULL
 *          1 otherwise
 */
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
/* =========================================================================
 * Function: append_string
 *    Securely append data_len bytes of data at the end of string->buffer
 *    The size of destination buffer is increased if needed.
 *
 * string_t *string: a pointer on the structure that will get appended data
 * const char *data: the src string
 * int data_len: the amount of bytes to add from data to string->buffer
 *
 * returns: -1 if reallocation failed
 *          the amount of copied bytes otherwise
 */
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

/* =========================================================================
 * Function: reset_string
 *    Erase data contained itn string->buffer and set string len to 0
 *
 * string_t *string: a pointer on a string structure
 *
 * returns: 0 if string point to NULL
 *          1 otherwise
 */
int reset_string(string_t *string) {
  if (!string) {
    fprintf(stderr, "[%s:%d] String appears to be empty, unable to reset\n",
            __FILE__, __LINE__);
    return 0;
  }

  bzero(string->buffer, string->bsize);
  string->len = 0;

  return 1;
}

/* =========================================================================
 * Function: revert_string
 *    Revert a string
 *
 * string_t *string: a pointer on a string structure
 *
 * returns: 0 if string point to NULL
 *          1 otherwise
 */
int revert_string(string_t *string) {
  char *left, *right, tmp;

  if (!string) {
    fprintf(stderr, "[%s:%d] String appears to be empty, unable to revert\n",
            __FILE__, __LINE__);
    return 0;
  }

  left = string->buffer;
  right = string->buffer + string->len - 1;

  while (left < right) {
    tmp = *left;
    *left = *right;
    *right = tmp;

    left++;
    right--;
  }

  return 1;
}

/* =========================================================================
 * Function: read_line_from_file
 *    Securely reads a line of a file and store it into string->buffer.
 *    The line is read using CHUNK_SIZE bytes at a time. If string->buffer
 *    is not large enough, its size is doubled as amany times as required to
 *    store the line.
 *
 * string_t *string: a pointer on a string structure
 * FILE *file: the file descriptor from which a new line is read
 *
 * returns: -1 if an error occurs
 *          the number of read bytes otherwise
 */
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
