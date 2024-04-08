#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_lines(char *path, char **lines);
ssize_t get_op(char* line, char* op);

int main(int argc, char **argv) {
  char **lines;
  int number_of_lines = read_lines(argv[1], lines);
  int32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15,
      x16, x17, x18, x19, x20, x21, x22, x23, x24, x25, x26, x27, x28, x29, x30,
      x31, pc;
  for (int i = 0; i < number_of_lines; ++i) {
    char *op;
    size_t op_size = get_op(lines[i], op);
    if (strcmp(op, "addi")) {

    }
  }
}

int read_lines(char *path, char **lines) {
  FILE *fp = fopen(path, "r");
  char *line = NULL;
  size_t len = 0;
  int number_of_lines = 0;
  int capacity = 2;
  lines = (char **)malloc(capacity * sizeof(char *));
  ssize_t read; // TODO: Delete
  while ((read = getline(&lines[number_of_lines++], &len, fp)) != -1) {
    if (number_of_lines >= capacity) {
      capacity *= 2;
      lines = realloc(lines, capacity * sizeof(char *));
    }
  }
  lines = realloc(lines, number_of_lines * sizeof(char *));
  return number_of_lines;
}

ssize_t get_op(char* line, char* op) {
    size_t len = strlen(line);
    for (int j = 0; j < len; ++j) {
      if (line[j] != ' ')
        continue;
      op = (char *)malloc((j + 1) * sizeof(char));
      strncpy(op, line, j);
      op[j + 1] = '\0';
      return j;
    }
    return -1;
}
