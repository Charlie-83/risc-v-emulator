#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum regs {
    x0,
    x1,
    x2,
    x3,
    x4,
    x5,
    x6,
    x7,
    x8,
    x9,
    x10,
    x11,
    x12,
    x13,
    x14,
    x15,
    x16,
    x17,
    x18,
    x19,
    x20,
    x21,
    x22,
    x23,
    x24,
    x25,
    x26,
    x27,
    x28,
    x29,
    x30,
    x31,
    pc,
};
struct R {
    enum regs rd;
    enum regs rs1;
    enum regs rs2;
};
struct I {
    enum regs rd;
    enum regs rs1;
    int16_t imm;
};
struct S {
    enum regs rs1;
    enum regs rs2;
    int16_t imm;
};
struct B {
    enum regs rs1;
    enum regs rs2;
    int16_t imm;
};
struct U {
    enum regs rd;
    int32_t imm;
};
struct J {
    enum regs rd;
    int32_t imm;
};

int read_lines(char *path, char **lines[]);
ssize_t get_op(char *line, char **op);
ssize_t getI(char *line, int index, struct I *i_struct);
ssize_t get_reg(char *line, int index, enum regs *reg);
ssize_t get_imm16(char *line, int i, int16_t *imm);

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
            printf("addi %d, %d, %d\n", operands.rd, operands.rs1,
                   operands.imm);
            regs[operands.rd] = regs[operands.rs1] + operands.imm;
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
        (*op)[j + 1] = '\0';
        return j;
    }
    printf("Couldn't find op in line %s", line);
    return -1;
}

ssize_t getI(char *line, int i, struct I *i_struct) {
    size_t len = strlen(line);
    enum regs rd, rs1;
    int16_t imm;
    ssize_t reg_len = get_reg(line, i, &rd);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    reg_len = get_reg(line, i, &rs1);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    char *endptr;
    imm = strtol(line + i, &endptr, 10);
    i_struct->rd = rd;
    i_struct->rs1 = rs1;
    i_struct->imm = imm;
    return i;
}

ssize_t get_reg(char *line, int i, enum regs *reg) {
    size_t len = strlen(line);
    int reg_len = 0;
    while (line[i] == ' ') {
        ++i;
        ++reg_len;
    }
    if (strncmp("x0", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x0;
    } else if (strncmp("x1", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else if (strncmp("x2", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x2;
    } else if (strncmp("x3", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x3;
    } else if (strncmp("x4", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x4;
    } else if (strncmp("x5", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x5;
    } else if (strncmp("x6", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x6;
    } else if (strncmp("x7", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else if (strncmp("x8", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x8;
    } else if (strncmp("x9", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x9;
    } else if (strncmp("x10", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x10;
    } else if (strncmp("x11", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x11;
    } else if (strncmp("x12", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x12;
    } else if (strncmp("x13", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x13;
    } else if (strncmp("x14", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x14;
    } else if (strncmp("x15", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x15;
    } else if (strncmp("x16", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x16;
    } else if (strncmp("x17", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x17;
    } else if (strncmp("x18", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x18;
    } else if (strncmp("x19", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x19;
    } else if (strncmp("x20", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x20;
    } else if (strncmp("x21", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x21;
    } else if (strncmp("x22", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x22;
    } else if (strncmp("x23", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x23;
    } else if (strncmp("x24", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x24;
    } else if (strncmp("x25", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x25;
    } else if (strncmp("x26", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x26;
    } else if (strncmp("x27", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x27;
    } else if (strncmp("x28", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x28;
    } else if (strncmp("x29", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x29;
    } else if (strncmp("x30", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x30;
    } else if (strncmp("x31", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x31;
    } else if (strncmp("pc", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else {
        return -1;
    }
    return reg_len;
}
