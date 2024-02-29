#include "ssvm_op.h"
#include <stdio.h>

uint64_t* op_push(uint64_t* sp, FILE* fd, int* error) {
    uint64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        sp += sizeof(uint64_t);
        *sp = piece;
    }
    return sp;
}

uint64_t* op_pop(uint64_t* sp, FILE* fd, int* error) {
    *error = -5;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", COMMAND_POP);
    return sp;
}

uint64_t* op_print(uint64_t* sp, FILE* fd, int* error) {
    printf("%lx", *sp);
    return sp;
}

uint64_t* op_print_fp(union stack_pointer vm, FILE* fd, int* error) {
    printf("%lf", *vm.sp_f64);
    return vm.sp;
}

uint64_t* op_seek_sp(union stack_pointer vm, FILE* fd, int* error) {
    int64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        vm.sp += piece * sizeof(uint64_t);
    }
    return vm.sp;
}
