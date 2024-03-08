#define SSVM_FILL_OPCODE_MATRIX
#include "ssvm_op.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "ssvm_matrix.h"

void* op_push(void* sp, FILE* fd, int* error) {
    uint64_t* usp = sp;
    uint64_t piece;
    int bytes = fread(&piece, 8, 1, fd);
    if (bytes == 1) {
        usp += 1;
        *usp = piece;
    }
    return usp;
}

void* op_pop(void* sp, FILE* fd, int* error) {
    uint64_t** ptr = sp;
    uint64_t* value_ptr = sp;
    value_ptr -= sizeof(uint64_t);
    **ptr = *value_ptr;
    return sp - sizeof(uint64_t) * 2;
}

void* op_print(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    printf("%ld\n", *x);
    return sp;
}

void* op_print_fp(void* sp, FILE* fd, int* error) {
    double* x = sp;
    printf("%lf\n", *x);
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
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x + *y;
    return x;
}

void* op_right_shift(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x >> *y;
    return x;
}

void* op_to_fp_s(void* sp, FILE* fd, int* error) {
    int64_t* x = sp;
    double* result = sp;
    *result = *x;
    return sp;
}

void* op_sub(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x - *y;
    return x;
}

void* op_mul(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x * *y;
    return x;
}

void* op_div(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x / *y;
    return x;
}

void* op_rem(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x % *y;
    return x;
}

void* op_bitwise_and(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x & *y;
    return x;
}

void* op_bitwise_or(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x | *y;
    return x;
}

void* op_bitwise_xor(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x ^ *y;
    return x;
}

void* op_clone(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x += sizeof(uint64_t); // ?????????????????????
    *x = *y;
    return x;
}

void* op_take(void* sp, FILE* fd, int* error) {
    uint64_t** value_ptr = sp;
    uint64_t* value = sp;
    *value = **value_ptr;
    return sp;
}

void* op_fp_add(void* sp, FILE* fd, int* error) {
    double* x = sp;
    double* y = sp;
    x -= 1; // ?????????????????????
    *x = *x + *y;
    return x;
}

void* op_fp_sub(void* sp, FILE* fd, int* error) {
    double* x = sp;
    double* y = sp;
    x -= 1; // ?????????????????????
    *x = *x - *y;
    return x;
}

void* op_fp_mul(void* sp, FILE* fd, int* error) {
    double* x = sp;
    double* y = sp;
    x -= 1; // ?????????????????????
    *x = *x * *y;
    return x;
}

void* op_fp_div(void* sp, FILE* fd, int* error) {
    double* x = sp;
    double* y = sp;
    x -= 1; // ?????????????????????
    *x = *x / *y;
    return x;
}

void* op_fp_power(void* sp, FILE* fd, int* error) {
    double* x = sp;
    double* y = sp;
    x -= 1; // ?????????????????????
    *x = pow(*x, *y);
    return x;
}

void* op_fp_ceil(void* sp, FILE* fd, int* error) {
    double* x = sp;
    *x = ceil(*x);
    return sp;
}

void* op_fp_round(void* sp, FILE* fd, int* error) {
    double* x = sp;
    *x = round(*x);
    return sp;
}

void* op_malloc(void* sp, FILE* fd, int* error) {
    uint64_t* sp_u64 = sp;
    *sp_u64 = (uint64_t)malloc(*sp_u64);
    return sp;
}

void* op_load(void* sp, FILE* fd, int* error) {
    void** sp_ptr = (void*)sp;
    uint64_t n = 0;
    int bytes = fread(&n, 8, 1, fd);
    if (bytes == 1) {
        char buffer[n];
        fread(buffer, n, 1, fd);
        memcpy((void*)*sp_ptr, buffer, n);
    }
    return sp;
}

void* op_free(void* sp, FILE* fd, int* error) {
    void* ptr = *((void**)sp);
    free(ptr);
    return sp - sizeof(uint64_t);
}

void* op_omit(void* sp, FILE* fd, int* error) {
    return sp - sizeof(int64_t);
}

void* op_print_all(void* sp, FILE* fd, int* error) {
    /*
} else if (c == COMMAND_PRINT_ALL) {
    uint64_t *ptr = vm.sp;
    do {
        printf("0x%x: 0x%x\n", vm.sp-ptr, *ptr);
        ptr -= sizeof(uint64_t);
    } while(ptr != stack);
 */
    return op_stub(sp, fd, error);
}

void* op_left_shift(void* sp, FILE* fd, int* error) {
   uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t); // ?????????????????????
    *x = *x << *y;
    return x;
}

void* op_swap(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    uint64_t* y = sp;
    x -= sizeof(uint64_t);

    uint64_t value = *x;
    *x = *y;
    *y = value;
    return sp;
}

void* op_to_fp(void* sp, FILE* fd, int* error) {
    uint64_t* x = sp;
    double* result = sp;
    *result = *x;
    return sp;
}

void* op_to_integer(void* sp, FILE* fd, int* error) {
    int64_t* result = sp;
    double* x = sp;
    *result = *x;
    return sp;
}

void* op_assert(void* sp, FILE* fd, int* error) {
    uint64_t* x = (sp-sizeof(uint64_t));
    uint64_t* y = sp;
    uint64_t z = *x;
    uint64_t t = *y;
    if (z == t) {

    } else {
        printf("%ld != %ld\n", z, t);
        *error = -16;
    }
    return sp;
}

void* op_call(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;

    struct vm_state vm;
    vm.operand_size = sizeof(uint64_t);
    vm.sp = sp;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (offset == 0) {
            *error = -6;
            return sp;
        }

        fseek(fd, pos + offset, SEEK_SET);

        *error = ssvm_matrix_call(vm, fd, sp);
        if (*error == -42) {
            *error = 0;
        }
        fseek(fd, pos + 9, SEEK_SET);
    }
    return sp;
}

void* op_ret(void* sp, FILE* fd, int* error) {
    *error = -42;
    return sp;
}

void* op_jump(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        if (offset == 0) {
            *error = -6;
            return sp;
        }
        fseek(fd, pos + offset, SEEK_SET);
    }
    return sp;
}

void* op_jump_e(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr == 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

void* op_jump_ne(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr != 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

void* op_jump_g(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr > 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

void* op_jump_ge(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr >= 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

void* op_jump_l(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr < 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

void* op_jump_le(void* sp, FILE* fd, int* error) {
    long pos = ftell(fd) - 1;
    uint64_t offset;
    int bytes = fread(&offset, 8, 1, fd);
    if (bytes == 1) {
        int64_t* value_ptr = sp;
        if (*value_ptr <= 0) {
            if (offset == 0) {
                *error = -6;
                return sp;
            }
            fseek(fd, pos + offset, SEEK_SET);
        }
    }
    return sp;
}

// fp comparison  ?

void* op_read_char(void* sp, FILE* fd, int* error) {
    sp += sizeof(uint64_t);
    int64_t* char_ptr = sp;
    *char_ptr = getchar();
    return sp;
}

void* op_read_integer(void* sp, FILE* fd, int* error) {
    uint64_t* usp = sp;
    usp += sizeof(uint64_t);
    scanf("%ld", usp);
    return usp;
}

void* op_read_floating(void* sp, FILE* fd, int* error) {
    sp += sizeof(uint64_t);
    scanf("%lf", sp);
    return sp;
}

void* op_read_binary_integer(void* sp, FILE* fd, int* error) {
    sp += sizeof(uint64_t);
    fread(sp, sizeof(uint64_t), 1, fd);
    return sp;
}

void* op_read_binary_floating(void* sp, FILE* fd, int* error) {
    sp += sizeof(uint64_t);
    fread(sp, sizeof(uint64_t), 1, fd);
    return sp;
}

void* op_print_string(void* sp, FILE* fd, int* error) {
    char** str_ptr = sp;
    printf("%s", *str_ptr);
    return sp;
}

void* op_print_char(void* sp, FILE* fd, int* error) {
    char* char_ptr = sp;
    putchar(*char_ptr);
    return sp;
}

void* op_compare_fp(void* sp, FILE* fd, int* error) {
    return op_stub(sp, fd, error);
    /*double* left_double = sp-sizeof(uint64_t);
    double* right_double = sp;
    int64_t* result = sp;
    if (*left_double > *right_double) {
        *result = 1;
    } else if (*left_double < *right_double) {
        *result = -1;
    } else if (*left_double == *right_double) {
        *result = 0;
    } else if (*left_double != *right_double) {
        *result = 0;
    }
    return sp;*/
}

void* op_eof(void* sp, FILE* fd, int* error) {
    int64_t* result = sp;
    *result = feof(stdin);
    return sp;
}

void* op_is_nan(void* sp, FILE* fd, int* error) {
    double* double_ptr = sp;
    int64_t* result = sp;
    *result = isnan(*double_ptr);
    return sp;
}

void* op_stub(void* sp, FILE* fd, int* error) {
    *error = -8;
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
