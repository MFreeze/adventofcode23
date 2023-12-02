#include "dynstring.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBS "0123456789"

int main(int argc, char **argv) {
  int ret = EXIT_SUCCESS;
  long int sum = 0;
  string_t content;
  FILE *input;

  char *ldigits[] = {"zero", "one", "two",   "three", "four",
                     "five", "six", "seven", "eight", "nine"};
  char *rdigits[] = {"orez", "eno", "owt",   "eerht", "ruof",
                     "evif", "xis", "neves", "thgie", "enin"};

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
    int found_nb, computed_nb = 0;
    char **search = ldigits;
    for (int j = 0, mul = 10; j < 2; j++, mul /= 10) {
      char *first = strpbrk(content.buffer, NBS);
      if (first) {
        found_nb = *first - 48;
        for (int i = 0; i < 10; i++) {
          char *sub;
          if ((sub = strstr(content.buffer, search[i])) != NULL) {
            if (sub < first) {
              first = sub;
              found_nb = i;
            }
          }
        }
      }
      computed_nb += found_nb * mul;
      search = rdigits;
      revert_string(&content);
    }
    reset_string(&content);

    sum += computed_nb;
  }

  printf("Output: %ld\n", sum);

  fclose(input);
FREE_STR:
  free_string(&content);
EXIT:
  return ret;
}
