#ifndef SSVM_OP_H
#define SSVM_OP_H

#include "ssvm.h"
#include <stdio.h>

union ssvm_matrix_friend {
    uint64_t* sp;
    double* sp_f64;
    void* ptr;
};

typedef void* (*ssvm_atom)(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_push(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_pop(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_print(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_seek_sp(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_add(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_sub(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_mul(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_div(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_rem(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_bitwise_and(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_bitwise_or(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_bitwise_xor(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_clone(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_omit(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_call(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_ret(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_take(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_fp_add(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_sub(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_mul(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_div(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_power(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_ceil(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_fp_round(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_malloc(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_load(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_free(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_print_all(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_print_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_jump(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_e(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_ne(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_g(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_ge(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_l(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_le(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_right_shift(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_swap(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_left_shift(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_to_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_to_fp_s(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_to_integer(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_assert(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_read_char(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_read_integer(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_read_floating(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_read_binary_integer(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_read_binary_floating(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_print_string(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_print_char(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_compare_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_eof(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_is_nan(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_jump_e_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_ne_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_g_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_l_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_nan_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_jump_not_nan_fp(union ssvm_matrix_friend arg, FILE* fd, int* error);

void* op_stub(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_not_implemented(union ssvm_matrix_friend arg, FILE* fd, int* error);
void* op_unknown(union ssvm_matrix_friend arg, FILE* fd, int* error);

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
    [COMMAND_CALL] = op_call,
    [COMMAND_RET] = op_ret,
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
    [COMMAND_JUMP_IF_ZERO] = op_jump_e,
    [COMMAND_JUMP_IF_NON_ZERO] = op_jump_ne,
    [COMMAND_JUMP_IF_GREAT] = op_jump_g,
    [COMMAND_JUMP_IF_LESS] = op_jump_l,
    [COMMAND_JUMP_IF_GREAT_OR_EQUAL] = op_jump_ge,
    [COMMAND_JUMP_IF_LESS_OR_EQUAL] = op_jump_le,
    [COMMAND_RIGHT_SHIFT] = op_right_shift,
    [COMMAND_LEFT_SHIFT] = op_left_shift,
    [COMMAND_SWAP] = op_swap,
    [COMMAND_TO_FP] = op_to_fp,
    [COMMAND_TO_FP_S] = op_to_fp_s,
    [COMMAND_TO_INTEGER] = op_to_integer,
    [0x30] = op_unknown,
    [0x31] = op_unknown,
    [COMMAND_ASSERT] = op_assert,
    [COMMAND_READ_CHAR] = op_read_char,
    [COMMAND_READ_INTEGER] = op_read_integer,
    [COMMAND_READ_FLOATING] = op_read_floating,
    [COMMAND_READ_BINARY_INTEGER] = op_read_binary_integer,
    [COMMAND_READ_BINARY_FLOATING] = op_read_binary_floating,
    [COMMAND_PRINT_STRING] = op_print_string,
    [COMMAND_PRINT_CHAR] = op_print_char,
    [COMMAND_COMPARE_FP] = op_compare_fp,
    [COMMAND_EOF] = op_eof,
    [COMMAND_IS_NAN] = op_is_nan,
    [COMMAND_JUMP_IF_ZERO_FP] = op_jump_e_fp,
	[COMMAND_JUMP_IF_NON_ZERO_FP] = op_jump_ne_fp,
	[COMMAND_JUMP_IF_GREAT_FP] = op_jump_g_fp,
    [COMMAND_JUMP_IF_LESS_FP] = op_jump_l_fp,
    [COMMAND_JUMP_IF_NAN] = op_jump_nan_fp,
    [COMMAND_JUMP_IF_NOT_NAN] = op_jump_not_nan_fp,
    [(COMMAND_JUMP_IF_NOT_NAN + 1) ... 255] = op_unknown
};

#endif

#endif
