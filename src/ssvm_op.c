#define SSVM_FILL_OPCODE_MATRIX
#include "ssvm_op.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ssvm_matrix.h"

void* op_push(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        arg.sp += 1;
        *arg.sp = piece;
    }
    return arg.sp;
}


void* op_pop(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t** ptr = arg.ptr;
    uint64_t* value_ptr = arg.sp;
    value_ptr -= sizeof(uint64_t);
    **ptr = *value_ptr;
    return arg.ptr - sizeof(uint64_t) * 2;
}

void* op_print(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    printf("%ld\n", *arg.sp);
    return arg.ptr;
}

void* op_print_fp(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    printf("%lf\n", *arg.sp_f64);
    return arg.ptr;
}

void* op_seek_sp(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    int64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        arg.sp += piece;
    }
    return arg.ptr;
}

void* op_add(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x + *y;
    return x;
}

void* op_right_shift(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x >> *y;
    return x;
}

void* op_to_fp_s(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    int64_t* x = arg.sp;
    double* result = arg.sp_f64;
    *result = *x;
    return arg.ptr;
}

void* op_sub(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x - *y;
    return x;
}

void* op_mul(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x * *y;
    return x;
}

void* op_div(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x / *y;
    return x;
}

void* op_rem(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x % *y;
    return x;
}

void* op_bitwise_and(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x & *y;
    return x;
}

void* op_bitwise_or(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x | *y;
    return x;
}

void* op_bitwise_xor(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x ^ *y;
    return x;
}

void* op_clone(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x += 1; // ?????????????????????
    *x = *y;
    return x;
}

void* op_take(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t** value_ptr = (uint64_t**)arg.sp;
    uint64_t* value = arg.sp;
    *value = **value_ptr;
    return arg.ptr;
}

void* op_fp_add(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    double* y = arg.sp_f64;
    x -= 1; // ?????????????????????
    (*x) = (*x) + (*y);
    return x;
}

void* op_fp_sub(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    double* y = arg.sp_f64;
    x -= 1; // ?????????????????????
    (*x) = (*x) - (*y);
    return x;
}

void* op_fp_mul(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    double* y = arg.sp_f64;
    x -= 1;
    (*x) = (*x) * (*y);
    return x;
}

void* op_fp_div(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    double* y = arg.sp_f64;
    x -= 1;
    (*x) = (*x) / (*y);
    return x;
}

void* op_fp_power(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    double* y = arg.sp_f64;
    x -= 1;
    *x = pow(*x, *y);
    return x;
}

void* op_fp_ceil(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    *x = ceil(*x);
    return arg.ptr;
}

void* op_fp_round(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* x = arg.sp_f64;
    *x = round(*x);
    return arg.ptr;
}

void* op_malloc(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    *arg.sp = (uint64_t)malloc(*arg.sp);
    return arg.ptr;
}

void* op_load(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    void** sp_ptr = (void*)arg.ptr;
    uint64_t n = 0;
    int bytes = fread(&n, 8, 1, fd);
    if (bytes == 1) {
        char buffer[n];
        fread(buffer, n, 1, fd);
        memcpy((void*)*sp_ptr, buffer, n);
    }
    return arg.ptr;
}

void* op_free(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    void* ptr = *((void**)arg.ptr);
    free(ptr);
    return arg.ptr - sizeof(uint64_t);
}

void* op_omit(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    return arg.sp - 1;
}

void* op_print_all(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    /*
} else if (c == COMMAND_PRINT_ALL) {
    uint64_t *ptr = vm.sp;
    do {
        printf("0x%x: 0x%x\n", vm.sp-ptr, *ptr);
        ptr -= sizeof(uint64_t);
    } while(ptr != stack);
 */
    return op_stub(arg, fd, error);
}

void* op_left_shift(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1; // ?????????????????????
    *x = *x << *y;
    return x;
}

void* op_swap(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    uint64_t* y = arg.sp;
    x -= 1;

    uint64_t value = *x;
    *x = *y;
    *y = value;
    return arg.ptr;
}

void* op_to_fp(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp;
    double* result = arg.sp_f64;
    *result = *x;
    return arg.ptr;
}

void* op_to_integer(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    int64_t* result = arg.sp;
    double* x = arg.sp_f64;
    *result = *x;
    return arg.ptr;
}

void* op_assert(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t* x = arg.sp-1;
    uint64_t* y = arg.sp;
    if (*x != *y) {
        printf("%ld != %ld\n", *x, *y);
        *error = -16;
    }
    return arg.ptr;
}

void* op_call(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;

    struct vm_state vm;
    vm.operand_size = sizeof(uint64_t);
    vm.sp = arg.sp;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (offset == 0) {
            *error = -6;
            return arg.ptr;
        }

        fseek(fd, pos + offset, SEEK_SET);

        *error = ssvm_matrix_call(vm, fd, arg.sp);
        if (*error == -42) {
            *error = 0;
        }
        fseek(fd, pos + 9, SEEK_SET);
    }
    return arg.ptr;
}

void* op_ret(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    *error = -42;
    return arg.ptr;
}

void* op_jump(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (offset == 0) {
            *error = -6;
            return arg.ptr;
        }
        fseek(fd, pos + offset, SEEK_SET);
    }
    return arg.ptr;
}

void* op_jump_e(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp == 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_jump_ne(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp != 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_jump_g(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp > 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_jump_ge(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp >= 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_jump_l(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp < 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_jump_le(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (*arg.sp <= 0) {
            if (offset == 0) {
                *error = -6;
                return arg.ptr;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return arg.ptr;
}

void* op_read_char(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    arg.sp += 1;
    int64_t* char_ptr = arg.sp;
    *char_ptr = getchar();
    return arg.ptr;
}

void* op_read_integer(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    arg.sp += 1;
    scanf("%ld", arg.sp);
    return arg.sp;
}

void* op_read_floating(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    arg.sp_f64 += 1;
    scanf("%lf", arg.sp_f64);
    return arg.ptr;
}

void* op_read_binary_integer(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    arg.sp += 1;
    fread(arg.sp, sizeof(uint64_t), 1, fd);
    return arg.ptr;
}

void* op_read_binary_floating(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    arg.sp_f64 += 1;
    fread(arg.sp_f64, sizeof(uint64_t), 1, fd);
    return arg.ptr;
}

void* op_print_string(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    printf("%s", (char*)*arg.sp);
    return arg.ptr;
}

void* op_print_char(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    char* char_ptr = (char*)arg.sp;
    putchar(*char_ptr);
    return arg.ptr;
}

void* op_compare_fp(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    uint64_t value = 0;
    // char equal = *(vm.sp_f64-1) == *vm.sp_f64;
    char greater = *(arg.sp_f64-1) > *arg.sp_f64;
    char less = *(arg.sp_f64-1) < *arg.sp_f64;
    char not_equal = *(arg.sp_f64-1) != *arg.sp_f64;

    value = not_equal | (greater << 1) | (less << 2);
    // Equal - 0
    // Not Equal - 1
    // Greater - 3
    // Less - 5

    // NaN != NaN -> 1
    // NaN != x -> 1
    arg.sp += 1;
    *arg.sp = value;
    return arg.sp;
}

void* op_eof(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    int64_t* result = arg.sp;
    *result = feof(stdin);
    return arg.ptr;
}

void* op_is_nan(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    double* double_ptr = arg.sp_f64;
    int64_t* result = arg.sp;
    *result = isnan(*double_ptr);
    return arg.ptr;
}

void* op_stub(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    *error = -8;
    return arg.ptr;
}

void* op_not_implemented(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    *error = -5;
    return arg.ptr;
}

void* op_unknown(union ssvm_matrix_friend arg, FILE* fd, int* error) {
    *error = -4;
    return arg.ptr;
}
