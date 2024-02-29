#ifndef SSVM_OP_H
#define SSVM_OP_H

#include "ssvm.h"
#include <stdio.h>

typedef void* (*ssvm_atom)(void* sp, FILE* fd, int* error);

void* op_push(void* sp, FILE* fd, int* error);
void* op_pop(void* sp, FILE* fd, int* error);
void* op_print(void* sp, FILE* fd, int* error);
void* op_print_fp(void* sp, FILE* fd, int* error);
void* op_seek_sp(void* sp, FILE* fd, int* error);

void* op_add(void* sp, FILE* fd, int* error);
void* op_sub(void* sp, FILE* fd, int* error);
void* op_mul(void* sp, FILE* fd, int* error);
void* op_div(void* sp, FILE* fd, int* error);

void* op_take(void* sp, FILE* fd, int* error);

void* op_right_shift(void* sp, FILE* fd, int* error);
void* op_left_shift(void* sp, FILE* fd, int* error);

void* op_not_implemented(void* sp, FILE* fd, int* error);
void* op_unknown(void* sp, FILE* fd, int* error);

extern ssvm_atom opcode_matrix[256];

#ifdef SSVM_FILL_OPCODE_MATRIX

ssvm_atom opcode_matrix[256] = {
    [COMMAND_PUSH] = op_push,
    [COMMAND_POP] = op_pop,
    [COMMAND_PRINT] = op_print,
    [COMMAND_PRINT_FP] = op_print_fp,
    [COMMAND_SEEK_SP] = op_seek_sp,
    [16] = op_unknown,
    [17] = op_unknown,
    [18] = op_unknown,
    [COMMAND_TAKE] = op_not_implemented,
    [20] = op_unknown,
    [21] = op_unknown,
    [COMMAND_FP_ADD ... COMMAND_JUMP_IF_LESS_OR_EQUAL] = op_not_implemented,
    [COMMAND_RIGHT_SHIFT] = op_right_shift,
    [COMMAND_LEFT_SHIFT ... COMMAND_LOAD_NATIVE_FN] = op_not_implemented,
    [(COMMAND_LOAD_NATIVE_FN + 1) ... 255] = op_unknown
};

#endif

#endif
