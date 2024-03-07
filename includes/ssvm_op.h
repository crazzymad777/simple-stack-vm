#ifndef SSVM_OP_H
#define SSVM_OP_H

#include "ssvm.h"
#include <stdio.h>

typedef void* (*ssvm_atom)(void* sp, FILE* fd, int* error);

void* op_push(void* sp, FILE* fd, int* error);
void* op_pop(void* sp, FILE* fd, int* error);
void* op_print(void* sp, FILE* fd, int* error);
void* op_seek_sp(void* sp, FILE* fd, int* error);

void* op_add(void* sp, FILE* fd, int* error);
void* op_sub(void* sp, FILE* fd, int* error);
void* op_mul(void* sp, FILE* fd, int* error);
void* op_div(void* sp, FILE* fd, int* error);
void* op_rem(void* sp, FILE* fd, int* error);

void* op_bitwise_and(void* sp, FILE* fd, int* error);
void* op_bitwise_or(void* sp, FILE* fd, int* error);
void* op_bitwise_xor(void* sp, FILE* fd, int* error);

void* op_clone(void* sp, FILE* fd, int* error);
void* op_omit(void* sp, FILE* fd, int* error);
// void* op_call(void* sp, FILE* fd, int* error);
// void* op_ret(void* sp, FILE* fd, int* error);

void* op_take(void* sp, FILE* fd, int* error);

void* op_fp_add(void* sp, FILE* fd, int* error);
void* op_fp_sub(void* sp, FILE* fd, int* error);
void* op_fp_mul(void* sp, FILE* fd, int* error);
void* op_fp_div(void* sp, FILE* fd, int* error);
void* op_fp_power(void* sp, FILE* fd, int* error);
void* op_fp_ceil(void* sp, FILE* fd, int* error);
void* op_fp_round(void* sp, FILE* fd, int* error);
void* op_malloc(void* sp, FILE* fd, int* error);
void* op_load(void* sp, FILE* fd, int* error);
void* op_free(void* sp, FILE* fd, int* error);
void* op_print_all(void* sp, FILE* fd, int* error);
void* op_print_fp(void* sp, FILE* fd, int* error);

void* op_jump(void* sp, FILE* fd, int* error);
void* op_jump_e(void* sp, FILE* fd, int* error);
void* op_jump_ne(void* sp, FILE* fd, int* error);
void* op_jump_g(void* sp, FILE* fd, int* error);
void* op_jump_ge(void* sp, FILE* fd, int* error);
void* op_jump_l(void* sp, FILE* fd, int* error);
void* op_jump_le(void* sp, FILE* fd, int* error);

void* op_right_shift(void* sp, FILE* fd, int* error);
void* op_swap(void* sp, FILE* fd, int* error);
void* op_left_shift(void* sp, FILE* fd, int* error);
void* op_to_fp(void* sp, FILE* fd, int* error);
void* op_to_fp_s(void* sp, FILE* fd, int* error);
void* op_to_integer(void* sp, FILE* fd, int* error);

void* op_call_c(void* sp, FILE* fd, int* error);
void* op_load_native_fn(void* sp, FILE* fd, int* error);
void* op_assert(void* sp, FILE* fd, int* error);

void* op_print_string(void* sp, FILE* fd, int* error);

void* op_stub(void* sp, FILE* fd, int* error);
void* op_not_implemented(void* sp, FILE* fd, int* error);
void* op_unknown(void* sp, FILE* fd, int* error);

extern ssvm_atom opcode_matrix[256];

#ifdef SSVM_FILL_OPCODE_MATRIX

ssvm_atom opcode_matrix[256] = {
    [COMMAND_PUSH] = op_push,
    [COMMAND_POP] = op_pop,
    [COMMAND_PRINT] = op_print,
    [COMMAND_SEEK_SP] = op_seek_sp,
    [COMMAND_ADD] = op_add,
    [COMMAND_SUB] = op_sub,
    [COMMAND_MUL] = op_mul,
    [COMMAND_DIV] = op_div,
    [COMMAND_REM] = op_rem,
    [COMMAND_BITWISE_AND] = op_bitwise_and,
    [COMMAND_BITWISE_OR] = op_bitwise_or,
    [COMMAND_BITWISE_XOR] = op_bitwise_xor,
    [COMMAND_CLONE] = op_clone,
    [COMMAND_OMIT] = op_omit,
    [COMMAND_CALL] = op_not_implemented,
    [COMMAND_RET] = op_not_implemented,
    [16] = op_unknown,
    [17] = op_unknown,
    [18] = op_unknown,
    [COMMAND_TAKE] = op_take,
    [20] = op_unknown,
    [21] = op_unknown,
    [COMMAND_FP_ADD] = op_fp_add,
    [COMMAND_FP_SUB] = op_fp_sub,
    [COMMAND_FP_MUL] = op_fp_mul,
    [COMMAND_FP_DIV] = op_fp_div,
    [COMMAND_FP_POWER] = op_fp_power,
    [COMMAND_FP_CEIL] = op_fp_ceil,
    [COMMAND_FP_ROUND] = op_fp_round,
    [COMMAND_MALLOC] = op_malloc,
    [COMMAND_LOAD] = op_load,
    [COMMAND_FREE] = op_free,
    [COMMAND_PRINT_ALL] = op_print_all,
    [COMMAND_PRINT_FP] = op_print_fp,
    [COMMAND_JUMP] = op_jump,
    [COMMAND_JUMP_IF_ZERO ... COMMAND_JUMP_IF_LESS_OR_EQUAL] = op_not_implemented,
    [COMMAND_RIGHT_SHIFT] = op_right_shift,
    [COMMAND_LEFT_SHIFT] = op_left_shift,
    [COMMAND_SWAP] = op_swap,
    [COMMAND_TO_FP] = op_to_fp,
    [COMMAND_TO_FP_S] = op_to_fp_s,
    [COMMAND_TO_INTEGER] = op_to_integer,
    [COMMAND_CALL_C] = op_call_c,
    [COMMAND_LOAD_NATIVE_FN] = op_load_native_fn,
    [COMMAND_ASSERT] = op_assert,
    [COMMAND_READ_CHAR] = op_not_implemented,
    [COMMAND_READ_INTEGER] = op_not_implemented,
    [COMMAND_READ_FLOATING] = op_not_implemented,
    [COMMAND_READ_BINARY_INTEGER] = op_not_implemented,
    [COMMAND_READ_BINARY_FLOATING] = op_not_implemented,
    [COMMAND_PRINT_STRING] = op_print_string,
    [COMMAND_PRINT_CHAR] = op_not_implemented,
    [(COMMAND_PRINT_CHAR + 1) ... 255] = op_unknown
};

#endif

#endif
