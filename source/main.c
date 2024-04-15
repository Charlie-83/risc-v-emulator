#include "decode.h"
#include "instructions.h"
#include "regs.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int read_lines(char *path, char **lines[]);
ssize_t get_op(char *line, enum instruction *op);

int main(int argc, char **argv) {
    char **lines;
    int number_of_lines = read_lines(argv[1], &lines);
    int32_t regs[pc + 1] = {0};
    int8_t memory[1024] = {0};
    while (regs[pc] < number_of_lines * 4) {
        regs[x0] = 0;
        int i = regs[pc] / 4;
        regs[pc] += 4;
        enum instruction op;
        size_t op_size = get_op(lines[i], &op);
        switch (op) {
        case add:
        case sub:
        case _xor:
        case _or:
        case _and:
        case sll:
        case srl:
        case sra:
        case slt:
        case sltu: {
            struct R opnd;
            ssize_t len = getR(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            switch (op) {
            case add:
                regs[opnd.rd] = regs[opnd.rs1] + regs[opnd.rs2];
                break;
            case sub:
                regs[opnd.rd] = regs[opnd.rs1] - regs[opnd.rs2];
                break;
            case _xor:
                regs[opnd.rd] = regs[opnd.rs1] ^ regs[opnd.rs2];
                break;
            case _or:
                regs[opnd.rd] = regs[opnd.rs1] | regs[opnd.rs2];
                break;
            case _and:
                regs[opnd.rd] = regs[opnd.rs1] & regs[opnd.rs2];
                break;
            case sll:
                regs[opnd.rd] = regs[opnd.rs1] << regs[opnd.rs2];
                break;
            case srl:
                regs[opnd.rd] = regs[opnd.rs1] << regs[opnd.rs2];
                break;
            case sra:
                regs[opnd.rd] = regs[opnd.rs1] << regs[opnd.rs2];
                break;
            case slt:
                regs[opnd.rd] = regs[opnd.rs1] < regs[opnd.rs2] ? 1 : 0;
                break;
            case sltu:
                regs[opnd.rd] = regs[opnd.rs1] << regs[opnd.rs2] ? 1 : 0;
                break;
            }
            break;
        }
        case addi:
        case xori:
        case ori:
        case andi:
        case slli:
        case srli:
        case srai:
        case slti:
        case sltiu:
        case lb:
        case lh:
        case lw:
        case lbu:
        case lhu:
        case jalr:
        case ecall:
        case ebreak: {
            struct I opnd;
            ssize_t len = getI(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            switch (op) {
            case addi:
                regs[opnd.rd] = regs[opnd.rs1] + opnd.imm;
                break;
            case xori:
                regs[opnd.rd] = regs[opnd.rs1] ^ opnd.imm;
                break;
            case ori:
                regs[opnd.rd] = regs[opnd.rs1] | opnd.imm;
                break;
            case andi:
                regs[opnd.rd] = regs[opnd.rs1] & opnd.imm;
                break;
            case slli:
                regs[opnd.rd] = regs[opnd.rs1] << opnd.imm;
                break;
            case srli:
                regs[opnd.rd] = regs[opnd.rs1] >> opnd.imm;
                break;
            case srai:
                regs[opnd.rd] = regs[opnd.rs1] >> opnd.imm;
                break;
            case slti:
                regs[opnd.rd] = regs[opnd.rs1] < opnd.imm ? 1 : 0;
                break;
            case sltiu:
                regs[opnd.rd] = regs[opnd.rs1] < opnd.imm ? 1 : 0;
                break;
            case lb:
                regs[opnd.rd] = memory[regs[opnd.rs1] + opnd.imm];
                break;
            case lh: {
                size_t address = regs[opnd.rs1] + opnd.imm;
                if (address & 1) {
                    printf("lh not aligned: %s", lines[i]);
                    return -1;
                }
#ifdef BIG_ENDIAN
                regs[opnd.rd] = (int32_t)memory[address] << 8;
                regs[opnd.rd] += (int32_t)memory[address + 1];
#else
                regs[opnd.rd] = (int32_t)memory[address + 1] << 8;
                regs[opnd.rd] += (int32_t)memory[address];
#endif // BIG_ENDIAN
                break;
            }
            case lw: {
                size_t address = regs[opnd.rs1] + opnd.imm;
                if (address & 3) {
                    printf("lw not aligned: %s", lines[i]);
                    return -1;
                }
#ifdef BIG_ENDIAN
                regs[opnd.rd] = (int32_t)memory[address] << 24;
                regs[opnd.rd] += (int32_t)memory[address + 1] << 16;
                regs[opnd.rd] += (int32_t)memory[address + 2] << 8;
                regs[opnd.rd] += (int32_t)memory[address + 3];
#else
                regs[opnd.rd] = (int32_t)memory[address + 3] << 24;
                regs[opnd.rd] += (int32_t)memory[address + 2] << 16;
                regs[opnd.rd] += (int32_t)memory[address + 1] << 8;
                regs[opnd.rd] += (int32_t)memory[address];
#endif // BIG_ENDIAN
                break;
            }
            case lbu:
                regs[opnd.rd] = memory[regs[opnd.rs1] + opnd.imm] & 0xAA;
                break;
            case lhu: {
                size_t address = regs[opnd.rs1] + opnd.imm;
                if (address & 1) {
                    printf("lh not aligned: %s", lines[i]);
                    return -1;
                }
#ifdef BIG_ENDIAN
                regs[opnd.rd] = (int32_t)memory[address] << 8;
                regs[opnd.rd] += (int32_t)memory[address + 1];
#else
                regs[opnd.rd] = (int32_t)memory[address + 1] << 8;
                regs[opnd.rd] += (int32_t)memory[address];
#endif // BIG_ENDIAN
                regs[opnd.rd] &= 0xAAAA;
                break;
            }
            case jalr:
                if (opnd.imm ^ 1) {
                    printf("jal jump would lead to misalignment of pc: %s", lines[i]);
                    return -1;
                }
                regs[opnd.rd] = regs[pc];
                regs[pc] = regs[opnd.rs1] + opnd.imm * 2;
                break;
            case ecall:
                break;
            case ebreak:
                break;
            }
            break;
        case sb:
        case sh:
        case sw: {
            struct S opnd;
            ssize_t len = getS(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            switch (op) {
            case sb:
                memory[regs[opnd.rs1] + opnd.imm] = regs[opnd.rs2];
                break;
            case sh: {
                size_t address = regs[opnd.rs1] + opnd.imm;
                if (address & 1) {
                    printf("sh not aligned: %s", lines[i]);
                    return -1;
                }
#ifdef BIG_ENDIAN
                memory[address] = regs[opnd.rs2] >> 8;
                memory[address + 1] = regs[opnd.rs2];
#else
                memory[address + 1] = regs[opnd.rs2] >> 8;
                memory[address] = regs[opnd.rs2];
#endif // BIG_ENDIAN
                break;
            }
            case sw: {

                size_t address = regs[opnd.rs1] + opnd.imm;
                if (address & 3) {
                    printf("sw not aligned: %s", lines[i]);
                    return -1;
                }
#ifdef BIG_ENDIAN
                memory[address] = regs[opnd.rs2] >> 24;
                memory[address + 1] = regs[opnd.rs2] >> 16;
                memory[address + 2] = regs[opnd.rs2] >> 8;
                memory[address + 3] = regs[opnd.rs2];
#else
                memory[address + 3] = regs[opnd.rs2] >> 24;
                memory[address + 2] = regs[opnd.rs2] >> 16;
                memory[address + 1] = regs[opnd.rs2] >> 8;
                memory[address] = regs[opnd.rs2];
#endif // BIG_ENDIAN
                break;
            }
            }
            break;
        }
        case beq:
        case bne:
        case blt:
        case bge:
        case bltu:
        case bgeu: {
            struct B opnd;
            ssize_t len = getB(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            switch (op) {
            case beq:
                if (regs[opnd.rs1] == regs[opnd.rs2])
                    regs[pc] += opnd.imm - 4;
                break;
            case bne:
                if (regs[opnd.rs1] != regs[opnd.rs2])
                    regs[pc] += opnd.imm * 2 - 4;
                break;
            case blt:
                if (regs[opnd.rs1] < regs[opnd.rs2])
                    regs[pc] += opnd.imm * 2 - 4;
                break;
            case bge:
                if (regs[opnd.rs1] >= regs[opnd.rs2])
                    regs[pc] += opnd.imm * 2 - 4;
                break;
            case bltu:
                if ((uint32_t)regs[opnd.rs1] < (uint32_t)regs[opnd.rs2])
                    regs[pc] += opnd.imm * 2 - 4;
                break;
            case bgeu:
                if ((uint32_t)regs[opnd.rs1] >= (uint32_t)regs[opnd.rs2])
                    regs[pc] += opnd.imm * 2 - 4;
                break;
            }
            break;
        }
        case jal: {
            struct J opnd;
            ssize_t len = getJ(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            if (opnd.imm ^ 1) {
                printf("jal jump would lead to misalignment of pc: %s", lines[i]);
                return -1;
            }
            regs[opnd.rd] = regs[pc];
            regs[pc] += opnd.imm * 2;
            break;
        }
        case lui:
        case auipc: {
            struct U opnd;
            ssize_t len = getU(lines[i], op_size, &opnd);
            if (len < 0)
                return -1;
            switch (op) {
            case lui:
                regs[opnd.rd] = opnd.imm << 12;
                break;
            case auipc:
                regs[opnd.rd] = regs[pc] - 4 + (opnd.imm << 12);
                break;
            }
            break;
        }
        }
        }
    }
    for (int i = 0; i < pc + 1; ++i)
        printf("%d: %d\n", i, regs[i]);
}

int read_lines(char *path, char **lines[]) {
    FILE *fp = fopen(path, "r");
    char *line = NULL;
    size_t len = 0;
    int number_of_lines = 0;
    int capacity = 2;
    *lines = (char **)malloc(capacity * sizeof(char *));
    while (getline(&(*lines)[number_of_lines++], &len, fp) != -1) {
        if ((*lines)[number_of_lines-1][0] == '#')
            --number_of_lines;
        if (number_of_lines >= capacity) {
            capacity *= 2;
            *lines = (char **)realloc(*lines, capacity * sizeof(char *));
        }
    }
    *lines = (char **)realloc(*lines, number_of_lines * sizeof(char *));
    return number_of_lines - 1;
}

ssize_t get_op(char *line, enum instruction *op) {
    size_t len = strlen(line);
    for (int j = 0; j < len; ++j) {
        if (line[j] != ' ')
            continue;
        // TODO: Currently addi matches add
        if (strncmp(line, "add", j) == 0)
            *op = add;
        else if (strncmp(line, "sub", j) == 0)
            *op = sub;
        else if (strncmp(line, "xor", j) == 0)
            *op = _xor;
        else if (strncmp(line, "or", j) == 0)
            *op = _or;
        else if (strncmp(line, "and", j) == 0)
            *op = _and;
        else if (strncmp(line, "sll", j) == 0)
            *op = sll;
        else if (strncmp(line, "srl", j) == 0)
            *op = srl;
        else if (strncmp(line, "sra", j) == 0)
            *op = sra;
        else if (strncmp(line, "slt", j) == 0)
            *op = slt;
        else if (strncmp(line, "sltu", j) == 0)
            *op = sltu;
        else if (strncmp(line, "addi", j) == 0)
            *op = addi;
        else if (strncmp(line, "xori", j) == 0)
            *op = xori;
        else if (strncmp(line, "ori", j) == 0)
            *op = ori;
        else if (strncmp(line, "andi", j) == 0)
            *op = andi;
        else if (strncmp(line, "slli", j) == 0)
            *op = slli;
        else if (strncmp(line, "srli", j) == 0)
            *op = srli;
        else if (strncmp(line, "srai", j) == 0)
            *op = srai;
        else if (strncmp(line, "slti", j) == 0)
            *op = slti;
        else if (strncmp(line, "sltiu", j) == 0)
            *op = sltiu;
        else if (strncmp(line, "lb", j) == 0)
            *op = lb;
        else if (strncmp(line, "lh", j) == 0)
            *op = lh;
        else if (strncmp(line, "lw", j) == 0)
            *op = lw;
        else if (strncmp(line, "lbu", j) == 0)
            *op = lbu;
        else if (strncmp(line, "lhu", j) == 0)
            *op = lhu;
        else if (strncmp(line, "sb", j) == 0)
            *op = sb;
        else if (strncmp(line, "sh", j) == 0)
            *op = sh;
        else if (strncmp(line, "sw", j) == 0)
            *op = sw;
        else if (strncmp(line, "beq", j) == 0)
            *op = beq;
        else if (strncmp(line, "bne", j) == 0)
            *op = bne;
        else if (strncmp(line, "blt", j) == 0)
            *op = blt;
        else if (strncmp(line, "bge", j) == 0)
            *op = bge;
        else if (strncmp(line, "bltu", j) == 0)
            *op = bltu;
        else if (strncmp(line, "bgeu", j) == 0)
            *op = bgeu;
        else if (strncmp(line, "jal", j) == 0)
            *op = jal;
        else if (strncmp(line, "jalr", j) == 0)
            *op = jalr;
        else if (strncmp(line, "lui", j) == 0)
            *op = lui;
        else if (strncmp(line, "auipc", j) == 0)
            *op = auipc;
        else if (strncmp(line, "ecall", j) == 0)
            *op = ecall;
        else if (strncmp(line, "ebreak", j) == 0)
            *op = ebreak;
        return j;
    }
    printf("Couldn't find op in line %s", line);
    return -1;
}
