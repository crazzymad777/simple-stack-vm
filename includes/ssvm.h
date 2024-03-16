#ifndef SIMPLESTACKVM_H
#define SIMPLESTACKVM_H

#include <inttypes.h>
#include <stdlib.h>

#define COMMAND_PUSH 0x01
#define COMMAND_POP 0x02
#define COMMAND_PRINT 0x03
#define COMMAND_SEEK_SP 0x04
#define COMMAND_ADD 0x05
#define COMMAND_SUB 0x06
#define COMMAND_MUL 0x07
#define COMMAND_DIV 0x08
#define COMMAND_REM 0x09
#define COMMAND_BITWISE_AND 0x0a
#define COMMAND_BITWISE_OR 0x0b
#define COMMAND_BITWISE_XOR 0x0c
#define COMMAND_CLONE 0x0d // *sp+1 = *sp; sp += 1
#define COMMAND_OMIT 0x20 // sp -= 1
#define COMMAND_CALL 0x0e
#define COMMAND_RET 0x0f
#define COMMAND_TAKE 0x13 // dereference *sp and replace *sp
#define COMMAND_FP_ADD 0x16
#define COMMAND_FP_SUB 0x17
#define COMMAND_FP_MUL 0x18
#define COMMAND_FP_DIV 0x19
#define COMMAND_FP_POWER 0x1a
#define COMMAND_FP_CEIL 0x1b
#define COMMAND_FP_ROUND 0x1c
#define COMMAND_MALLOC 0x1d // *sp = sizeof heap -> returns pointer on stack: *sp = malloc(*sp)
#define COMMAND_LOAD 0x1e // vector
#define COMMAND_FREE 0x1f
#define COMMAND_PRINT_ALL 0x21
#define COMMAND_PRINT_FP 0x22

// Jumps
#define COMMAND_JUMP 0x23 // non-conditional jump: jump {offset in bytes}
#define COMMAND_JUMP_IF_ZERO 0x24 // jump if *sp = 0: je {offset in bytes}
#define COMMAND_JUMP_IF_NON_ZERO 0x25 // jump if *sp != 0: jne {offset in bytes}
#define COMMAND_JUMP_IF_GREAT 0x26 // jump if *sp > 0: jg
#define COMMAND_JUMP_IF_LESS 0x27 // jump if *sp < 0: jl
#define COMMAND_JUMP_IF_GREAT_OR_EQUAL 0x28 // jump if *sp > 0: jge
#define COMMAND_JUMP_IF_LESS_OR_EQUAL 0x29 // jump if *sp < 0: jle

#define COMMAND_RIGHT_SHIFT 0x2a
#define COMMAND_LEFT_SHIFT 0x2b
#define COMMAND_SWAP 0x2c

#define COMMAND_TO_FP 0x2d
#define COMMAND_TO_FP_S 0x2e // signed
#define COMMAND_TO_INTEGER 0x2f // convert floating point to integer

#define COMMAND_ASSERT 0x32

#define COMMAND_READ_CHAR 0x81
#define COMMAND_READ_INTEGER 0x82
#define COMMAND_READ_FLOATING 0x83
#define COMMAND_READ_BINARY_INTEGER 0x84
#define COMMAND_READ_BINARY_FLOATING 0x85

#define COMMAND_PRINT_STRING 0x86
#define COMMAND_PRINT_CHAR 0x87

#define COMMAND_COMPARE_FP 0x88
#define COMMAND_EOF 0x89
#define COMMAND_IS_NAN 0x8a

#define COMMAND_JUMP_IF_ZERO_FP 0x8b
#define COMMAND_JUMP_IF_NON_ZERO_FP 0x8c
#define COMMAND_JUMP_IF_GREAT_FP 0x8d
#define COMMAND_JUMP_IF_LESS_FP 0x8e
#define COMMAND_JUMP_IF_NAN 0x8f
#define COMMAND_JUMP_IF_NOT_NAN 0x90

#ifdef __cplusplus
extern "C" {
#endif

struct vm_state {
	union {
		uint64_t* sp; // stack pointer
		int64_t* sp_s; // stack pointer (signed value)
		double* sp_f64;
		uint64_t** sp_ptr;
		void** sp_fn_ptr;
	};
	size_t operand_size;
};

#ifdef __cplusplus
}
#endif

#endif

