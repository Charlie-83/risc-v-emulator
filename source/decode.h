#pragma once
#include <stdlib.h>
#include "regs.h"
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

ssize_t getR(char *line, int index, struct R *R_struct);
ssize_t getI(char *line, int index, struct I *I_struct);
ssize_t getS(char *line, int index, struct S *S_struct);
ssize_t getB(char *line, int index, struct B *B_struct);
ssize_t getU(char *line, int index, struct U *U_struct);
ssize_t getJ(char *line, int index, struct J *J_struct);
ssize_t get_reg(char *line, int index, enum regs *reg);
