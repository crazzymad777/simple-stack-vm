#ifndef SIMPLESTACKVM_H
#define SIMPLESTACKVM_H

#include <inttypes.h>
#include <stdlib.h>

#define COMMAND_PUSH 1 // uint64
#define COMMAND_POP 2 // uint64* // doubt. Because static address
#define COMMAND_PRINT 3 // -
#define COMMAND_SEEK_SP 4 // int64
#define COMMAND_ADD 5
#define COMMAND_SUB 6
#define COMMAND_MUL 7
#define COMMAND_DIV 8
#define COMMAND_REM 9
#define COMMAND_BITWISE_AND 10
#define COMMAND_BITWISE_OR 11
#define COMMAND_BITWISE_XOR 12
#define COMMAND_CLONE 13 // *sp+1 = *sp; sp += 1 
#define COMMAND_OMIT 32 // sp -= 1

#define COMMAND_CALL 14
// *sp = function index
// *(sp-n) = function arguments
// *(sp-n-m) = function return

// VM set SP on function return

#define COMMAND_RET 15

#define COMMAND_TAKE 19 // dereference *sp and replace *sp

// sysv-amd64 call

// float-pointing values
#define COMMAND_FP_ADD 22 // *(sp-1) = *(sp-1) + *sp ; sp -= 1
#define COMMAND_FP_SUB 23
#define COMMAND_FP_MUL 24
#define COMMAND_FP_DIV 25
#define COMMAND_FP_POWER 26
#define COMMAND_FP_CEIL 27
#define COMMAND_FP_ROUND 28
#define COMMAND_MALLOC 29 // *sp = sizeof heap -> returns pointer on stack: *sp = malloc(*sp)
#define COMMAND_LOAD 30 // uint64 N, N bytes
#define COMMAND_FREE 31 // *sp
//#define COMMAND_OMIT 32
#define COMMAND_PRINT_ALL 33
#define COMMAND_PRINT_FP 34

// Jumps
// Conditional jump

#define COMMAND_JUMP 35 // non-conditional jump: jump {offset in bytes}
#define COMMAND_JUMP_IF_ZERO 36 // jump if *sp = 0: je {offset in bytes}
#define COMMAND_JUMP_IF_NON_ZERO 37 // jump if *sp != 0: jne {offset in bytes}
#define COMMAND_JUMP_IF_GREAT 38 // jump if *sp > 0: jg
#define COMMAND_JUMP_IF_LESS 39 // jump if *sp < 0: jl
#define COMMAND_JUMP_IF_GREAT_OR_EQUAL 40 // jump if *sp > 0: jge
#define COMMAND_JUMP_IF_LESS_OR_EQUAL 41 // jump if *sp < 0: jle

#define COMMAND_RIGHT_SHIFT 42
#define COMMAND_LEFT_SHIFT 43
#define COMMAND_SWAP 44

#define COMMAND_TO_FP 45
#define COMMAND_TO_FP_S 46 // signed
#define COMMAND_TO_INTEGER 47 // convert floating point to integer

#define COMMAND_CALL_C 48 // *sp - function pointer, *(sp-1) arguments pointer, *(sp-2) size of arguments
// #define COMMAND_CALL_STD_SSVM 49 // index
#define COMMAND_LOAD_NATIVE_FN 49 // index, return pointer to function on stack
// 0 - dlopen - char* filename, int flag -> void
// 1 - dlerror - -> char*
// 2 - dlsym - void* handle, char* symbol
// 3 - dlclose - void* handle -> int
// 4 - puts

#define COMMAND_ASSERT 50

#define COMMAND_READ_CHAR 0x81
#define COMMAND_READ_INTEGER 0x82
#define COMMAND_READ_FLOATING 0x83
#define COMMAND_READ_BINARY_INTEGER 0x84
#define COMMAND_READ_BINARY_FLOATING 0x85

#define COMMAND_PRINT_STRING 0x86
#define COMMAND_PRINT_CHAR 0x87

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

#endif

