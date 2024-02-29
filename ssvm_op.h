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

extern ssvm_atom opcode_matrix[256];

#ifdef SSVM_FILL_OPCODE_MATRIX

ssvm_atom opcode_matrix[256] = {
    [COMMAND_PUSH] = op_push,
    [COMMAND_POP] = op_pop,
    [COMMAND_PRINT] = op_print,
    [COMMAND_PRINT_FP] = op_print_fp,
    [COMMAND_SEEK_SP] = op_seek_sp
};

#endif

#endif
