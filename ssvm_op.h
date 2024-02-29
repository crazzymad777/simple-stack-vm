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
    [COMMAND_ADD] = op_add,
    [COMMAND_SUB] = op_not_implemented,
    [COMMAND_MUL] = op_not_implemented,
    [COMMAND_DIV] = op_not_implemented,
    [COMMAND_REM] = op_not_implemented,
    [COMMAND_BITWISE_AND] = op_not_implemented,
    [COMMAND_BITWISE_OR] = op_not_implemented,
    [COMMAND_BITWISE_XOR] = op_not_implemented,
    [COMMAND_CLONE] = op_not_implemented,
    [COMMAND_OMIT] = op_not_implemented,
    [COMMAND_CALL] = op_not_implemented,
    [COMMAND_RET] = op_not_implemented,
    [16] = op_unknown,
    [17] = op_unknown,
    [18] = op_unknown,
    [COMMAND_TAKE] = op_not_implemented,
    [20] = op_unknown,
    [21] = op_unknown,
    [COMMAND_FP_ADD] = op_not_implemented,
    [COMMAND_FP_SUB] = op_not_implemented,
    [COMMAND_FP_MUL] = op_not_implemented,
    [COMMAND_FP_DIV] = op_not_implemented,
    [COMMAND_FP_POWER] = op_not_implemented,
    [COMMAND_FP_CEIL] = op_not_implemented,
    [COMMAND_FP_ROUND] = op_not_implemented,
    [COMMAND_MALLOC] = op_not_implemented,
    [COMMAND_LOAD] = op_not_implemented,
    [COMMAND_FREE] = op_not_implemented,
    [COMMAND_PRINT_ALL] = op_not_implemented,
    [COMMAND_PRINT_FP] = op_not_implemented,
    [COMMAND_JUMP] = op_not_implemented,
    [COMMAND_JUMP_IF_ZERO ... COMMAND_JUMP_IF_LESS_OR_EQUAL] = op_not_implemented,
    [COMMAND_RIGHT_SHIFT] = op_right_shift,
    [COMMAND_LEFT_SHIFT] = op_not_implemented,
    [COMMAND_SWAP] = op_not_implemented,
    [COMMAND_TO_FP] = op_not_implemented,
    [COMMAND_TO_FP_S] = op_not_implemented,
    [COMMAND_TO_INTEGER] = op_not_implemented,
    [COMMAND_CALL_C] = op_not_implemented,
    [COMMAND_LOAD_NATIVE_FN] = op_not_implemented,
    [(COMMAND_LOAD_NATIVE_FN + 1) ... 255] = op_unknown
};

#endif

#endif
