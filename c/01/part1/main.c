#include "dynstring.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBS "0123456789"

int main(int argc, char **argv) {
  int ret = EXIT_SUCCESS;
  long int sum = 0;
  char number[3] = {0};
  string_t content;
  FILE *input;

  if (argc != 2) {
    fprintf(stderr, "Program take input file as an argument\n");
    ret = EXIT_FAILURE;
    goto EXIT;
  }

  if (!init_string(&content)) {
    fprintf(stderr, "Cannot initialise content string\n");
    ret = EXIT_FAILURE;
    goto EXIT;
  }

  if ((input = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open file %s to read\n", argv[1]);
    goto FREE_STR;
  }

  while (read_line_from_file(&content, input) > 0) {
    char *first = strpbrk(content.buffer, NBS);
    if (first) {
      char *last = content.buffer + content.len;
      while (*last < '0' || *last > '9') {
        last--;
      }

      number[0] = *first;
      number[1] = *last;

      sum += atoi(number);
    }
    reset_string(&content);
  }

  printf("Output: %ld\n", sum);

FREE_STR:
  free_string(&content);
EXIT:
  return ret;
}
