#include "decode.h"
#include "regs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_lines(char *path, char **lines[]);
ssize_t get_op(char *line, char **op);

int main(int argc, char **argv) {
    char **lines;
    int number_of_lines = read_lines(argv[1], &lines);
    int32_t regs[pc + 1];
    for (int i = 0; i < number_of_lines; ++i) {
        char *op;
        size_t op_size = get_op(lines[i], &op);
        if (strcmp(op, "addi") == 0) {
            struct I operands;
            ssize_t len = getI(lines[i], op_size, &operands);
            if (len < 0)
                return -1;
            printf("addi %d, %d, %d\n", operands.rd, operands.rs1, operands.imm);
            regs[operands.rd] = regs[operands.rs1] + operands.imm;
        } else if (strcmp(op, "add") == 0) {
            struct R operands;
            ssize_t len = getR(lines[i], op_size, &operands);
            if (len < 0)
                return -1;
            printf("add %d, %d, %d\n", operands.rd, operands.rs1, operands.rs2);
            regs[operands.rd] = regs[operands.rs1] + regs[operands.rs2];
        } else if (strcmp(op, "sd") == 0) {
            struct S operands;
            ssize_t len = getS(lines[i], op_size, &operands);
            if (len < 0)
                return -1;
            printf("sd %d, (%d)%d\n", operands.rs1, operands.imm, operands.rs2);
        } else if (strcmp(op, "beq") == 0) {
            struct B operands;
            ssize_t len = getB(lines[i], op_size, &operands);
            if (len < 0)
                return -1;
            printf("beq %d, %d, %d\n", operands.rs1, operands.rs2, operands.imm);
        }
    }
}

int read_lines(char *path, char **lines[]) {
    FILE *fp = fopen(path, "r");
    char *line = NULL;
    size_t len = 0;
    int number_of_lines = 0;
    int capacity = 2;
    *lines = (char **)malloc(capacity * sizeof(char *));
    ssize_t read; // TODO: Delete
    while ((read = getline(&(*lines)[number_of_lines++], &len, fp)) != -1) {
        if (number_of_lines >= capacity) {
            capacity *= 2;
            *lines = (char **)realloc(*lines, capacity * sizeof(char *));
        }
    }
    *lines = (char **)realloc(*lines, number_of_lines * sizeof(char *));
    return number_of_lines;
}

ssize_t get_op(char *line, char **op) {
    size_t len = strlen(line);
    for (int j = 0; j < len; ++j) {
        if (line[j] != ' ')
            continue;
        *op = (char *)malloc((j + 1) * sizeof(char));
        strncpy(*op, line, j);
        (*op)[j] = '\0';
        return j;
    }
    printf("Couldn't find op in line %s", line);
    return -1;
}
