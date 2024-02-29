#include "ssvm_op.h"
#include <stdio.h>

void* op_push(void* sp, FILE* fd, int* error) {
    uint64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        sp += sizeof(uint64_t);
        uint64_t* value = sp;
        *value = piece;
    }
    return sp;
}

void* op_pop(void* sp, FILE* fd, int* error) {
    *error = -5;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", COMMAND_POP);
    return sp;
}

void* op_print(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    printf("%lx", x);
    return sp;
}

void* op_print_fp(void* sp, FILE* fd, int* error) {
    double* x = sp;
    printf("%lf", *x);
    return sp;
}

void* op_seek_sp(void* sp, FILE* fd, int* error) {
    int64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        sp += piece * sizeof(uint64_t);
    }
    return sp;
}
