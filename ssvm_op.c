#define SSVM_FILL_OPCODE_MATRIX
#include "ssvm_op.h"
#include <stdio.h>
#include <dlfcn.h>

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

void* op_add(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp-sizeof(uint64_t);
    uint64_t* y = sp;
    *x = *x + *y;
    sp = sp-sizeof(uint64_t);
    return sp;
}

void* op_right_shift(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp-sizeof(uint64_t);
    uint64_t* y = sp;
    *x = *x >> *y;
    sp = sp-sizeof(uint64_t);
    return sp;
}

void* op_to_fp_s(void* sp, FILE* fd, int* error) {
    double* x = sp;
    int64_t* result = sp;
    *result = *x;
    return sp;
}

void* op_load_native_fn(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    void **fn = sp;
    if (*x == 0) {
        *fn = dlopen;
    } else if (*x == 1) {
        *fn = dlerror;
    } else if (*x == 2) {
        *fn = dlsym;
    } else if (*x == 3) {
        *fn = dlclose;
    } else if (*x == 4) {
        *fn = puts;
    } else {
        *fn = NULL;
    }
    return sp;
}

void* op_not_implemented(void* sp, FILE* fd, int* error) {
    *error = -5;
    return sp;
}

void* op_unknown(void* sp, FILE* fd, int* error) {
    *error = -4;
    return sp;
}


/*if (c == COMMAND_SUB) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) - *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_MUL) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) * *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_DIV) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) / *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_REM) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) % *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_BITWISE_AND) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) & *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_BITWISE_OR) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) | *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_BITWISE_XOR) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) ^ *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_CLONE) {
    uint64_t cell = *vm.sp;
    vm.sp += sizeof(uint64_t);
    *vm.sp = cell;
} else if (c == COMMAND_OMIT) {
    vm.sp -= sizeof(uint64_t);
} else if (c == COMMAND_MALLOC) {
    *vm.sp = (uint64_t)malloc(*vm.sp);
} else if (c == COMMAND_FREE) {
    free(vm.sp);
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_PRINT_ALL) {
    uint64_t *ptr = vm.sp;
    do {
        printf("0x%x: 0x%x\n", vm.sp-ptr, *ptr);
        ptr -= sizeof(uint64_t);
    } while(ptr != stack);

} else if (c == COMMAND_FP_ADD) {
    *(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) + *vm.sp_f64;
    vm.sp = vm.sp-sizeof(uint64_t);

} else if (c == COMMAND_FP_SUB) {
    *(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) - *vm.sp_f64;
    vm.sp = vm.sp-sizeof(uint64_t);

} else if (c == COMMAND_FP_MUL) {
    *(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) * *vm.sp_f64;
    vm.sp = vm.sp-sizeof(uint64_t);

} else if (c == COMMAND_FP_DIV) {
    *(vm.sp_f64-sizeof(uint64_t)) = *(vm.sp_f64-sizeof(uint64_t)) / *vm.sp_f64;
    vm.sp = vm.sp-sizeof(uint64_t);

} else if (c == COMMAND_FP_POWER) {
    *(vm.sp_f64-sizeof(uint64_t)) = pow(*(vm.sp_f64-sizeof(uint64_t)), *vm.sp_f64);
    vm.sp = vm.sp-sizeof(uint64_t);

} else if (c == COMMAND_FP_CEIL) {
    *vm.sp_f64 = ceil(*vm.sp_f64);
} else if (c == COMMAND_FP_ROUND) {
    *vm.sp_f64 = round(*vm.sp_f64);
} else if (c == COMMAND_TAKE) {
    *vm.sp = **vm.sp_ptr;
} else if (c == COMMAND_LEFT_SHIFT) {
    *(vm.sp-sizeof(uint64_t)) = *(vm.sp-sizeof(uint64_t)) << *vm.sp;
    vm.sp = vm.sp-sizeof(uint64_t);
} else if (c == COMMAND_SWAP) {
    uint64_t value = *vm.sp;
    *vm.sp = *(vm.sp-sizeof(uint64_t));
    *(vm.sp-sizeof(uint64_t)) = value;
} else if (c == COMMAND_TO_FP) {
    *vm.sp_f64 = *vm.sp;
} else if (c == COMMAND_TO_FP_S) {
    *vm.sp_f64 = *vm.sp_s;
} else if (c == COMMAND_TO_INTEGER) {
    *vm.sp = *vm.sp_f64;
} else if (c == COMMAND_JUMP) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (offset == 0) {
            fprintf(stderr, "Simple Stack VM halt!\n");
            return -6;
        }
        fseek(fd, pos + offset, SEEK_SET);
    }
} else if (c == COMMAND_CALL_C) {
    // USE libffi
    *vm_ptr = vm;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
    return -5;
}else if (c == COMMAND_LOAD) {
    uint64_t n = 0;
    int bytes = fread(&n, 8, 1, fd);
    //printf("%d\n", n);
    if (bytes == 1) {
        //printf("%x\n", *vm.sp);

        char buffer[n];
        fread(buffer, n, 1, fd);
        memcpy((void*)*vm.sp, buffer, n);
    }
} else if (c == COMMAND_CALL) {
    *vm_ptr = vm;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
    return -5;
} else if (c == COMMAND_RET) {
    *vm_ptr = vm;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
    return -5;
} else if (c >= COMMAND_JUMP_IF_ZERO && c <= COMMAND_JUMP_IF_LESS_OR_EQUAL) {
    *vm_ptr = vm;
    fprintf(stderr, "Error! Not implemented opcode: 0x%x\n", c);
    return -5;
} else {
    *vm_ptr = vm;
    fprintf(stderr, "Error! Unknown opcode: 0x%x\n", c);
    return -4;
} */
