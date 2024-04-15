#include "decode.h"
#include <stdio.h>
#include <string.h>

ssize_t getR(char *line, int i, struct R *r_struct) {
    size_t len = strlen(line);
    enum regs rd, rs1, rs2;
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
    reg_len = get_reg(line, i, &rs2);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    r_struct->rd = rd;
    r_struct->rs1 = rs1;
    r_struct->rs2 = rs2;
    return i;
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

ssize_t getS(char *line, int i, struct S *s_struct) {
    size_t len = strlen(line);
    enum regs rs1, rs2;
    int16_t imm;
    ssize_t reg_len = get_reg(line, i, &rs1);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    char *endptr;
    imm = strtol(line + i, &endptr, 10);
    i += endptr - (line + i);
    if (line[i++] != '(') {
        printf("Expected ( at position %d in line: %s", i - 1, line);
        return -1;
    }
    reg_len = get_reg(line, i, &rs2);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ')') {
        printf("Expected ) at position %d in line: %s", i - 1, line);
        return -1;
    }
    s_struct->rs1 = rs1;
    s_struct->rs2 = rs2;
    s_struct->imm = imm;
    return i;
}

ssize_t getB(char *line, int i, struct B *b_struct) {
    size_t len = strlen(line);
    enum regs rs1, rs2;
    int16_t imm;
    ssize_t reg_len = get_reg(line, i, &rs1);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    reg_len = get_reg(line, i, &rs2);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    char *endptr;
    imm = strtol(line + i, &endptr, 10);
    b_struct->rs1 = rs1;
    b_struct->rs2 = rs2;
    b_struct->imm = imm;
    return i;
}

ssize_t getU(char *line, int i, struct U *u_struct) {
    size_t len = strlen(line);
    enum regs rd;
    int16_t imm;
    ssize_t reg_len = get_reg(line, i, &rd);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    char *endptr;
    imm = strtol(line + i, &endptr, 10);
    u_struct->rd = rd;
    u_struct->imm = imm;
    return i;
}

ssize_t getJ(char *line, int i, struct J *j_struct) {
    size_t len = strlen(line);
    enum regs rd;
    int16_t imm;
    ssize_t reg_len = get_reg(line, i, &rd);
    if (reg_len == -1)
        return -1;
    i += reg_len;
    if (line[i++] != ',') {
        printf("Expected comma at position %d in line: %s", i - 1, line);
        return -1;
    }
    char *endptr;
    imm = strtol(line + i, &endptr, 10);
    j_struct->rd = rd;
    j_struct->imm = imm;
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
    } else if (strncmp("zero", line + i, 4) == 0) {
        reg_len += 4;
        *reg = x0;
    } else if (strncmp("x4", line + i, 2) == 0 || strncmp("tp", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x4;
    } else if (strncmp("x5", line + i, 2) == 0 || strncmp("t0", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x5;
    } else if (strncmp("x6", line + i, 2) == 0 || strncmp("t1", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x6;
    } else if (strncmp("x7", line + i, 2) == 0 || strncmp("t2", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else if (strncmp("x8", line + i, 2) == 0 || strncmp("s0", line + i, 2) == 0 ||
               strncmp("fp", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x8;
    } else if (strncmp("x9", line + i, 2) == 0 || strncmp("s1", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x9;
    } else if (strncmp("x10", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x10;
    } else if (strncmp("a0", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x10;
    } else if (strncmp("x11", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x11;
    } else if (strncmp("a1", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x11;
    } else if (strncmp("x12", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x12;
    } else if (strncmp("a2", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x12;
    } else if (strncmp("x13", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x13;
    } else if (strncmp("a3", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x13;
    } else if (strncmp("x14", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x14;
    } else if (strncmp("a4", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x14;
    } else if (strncmp("x15", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x15;
    } else if (strncmp("a5", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x15;
    } else if (strncmp("x16", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x16;
    } else if (strncmp("a6", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x16;
    } else if (strncmp("x17", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x17;
    } else if (strncmp("a7", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x17;
    } else if (strncmp("x18", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x18;
    } else if (strncmp("s2", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x18;
    } else if (strncmp("x19", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x19;
    } else if (strncmp("s3", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x19;
    } else if (strncmp("x20", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x20;
    } else if (strncmp("s4", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x20;
    } else if (strncmp("x21", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x21;
    } else if (strncmp("s5", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x21;
    } else if (strncmp("x22", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x22;
    } else if (strncmp("s6", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x22;
    } else if (strncmp("x23", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x23;
    } else if (strncmp("s7", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x23;
    } else if (strncmp("x24", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x24;
    } else if (strncmp("s8", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x24;
    } else if (strncmp("x25", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x25;
    } else if (strncmp("s9", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x25;
    } else if (strncmp("x26", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x26;
    } else if (strncmp("s10", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x26;
    } else if (strncmp("x27", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x27;
    } else if (strncmp("s11", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x27;
    } else if (strncmp("x28", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x28;
    } else if (strncmp("t3", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x28;
    } else if (strncmp("x29", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x29;
    } else if (strncmp("t4", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x29;
    } else if (strncmp("x30", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x30;
    } else if (strncmp("t5", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x30;
    } else if (strncmp("x31", line + i, 3) == 0) {
        reg_len += 3;
        *reg = x31;
    } else if (strncmp("t6", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x31;
    } else if (strncmp("x1", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else if (strncmp("ra", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else if (strncmp("x2", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x2;
    } else if (strncmp("sp", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x2;
    } else if (strncmp("x3", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x3;
    } else if (strncmp("gp", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x3;
    } else if (strncmp("pc", line + i, 2) == 0) {
        reg_len += 2;
        *reg = x1;
    } else {
        return -1;
    }
    return reg_len;
}
