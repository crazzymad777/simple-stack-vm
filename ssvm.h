#ifndef SIMPLESTACKVM_H
#define SIMPLESTACKVM_H

#include <inttypes.h>

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

// Doubt
/*#define COMMAND_DLOPEN 16
#define COMMAND_DLERROR 17
#define COMMAND_DLSYM 18
#define COMMAND_DLCLOSE 18*/
// -----------

//#define COMMAND_TAKE 19 // derefernce uint64* and push on stack // doubt. Because static address

//#define COMMAND_POP_BY_POINTER 20 // **sp = *(sp-1); sp -= 2
//#define COMMAND_TAKE_BY_POINTER 21 // *sp = **sp

// sysv-amd64 call

// float-pointing values
#define COMMAND_FP_ADD 22 // *(sp-1) = *(sp-1) + *sp ; sp -= 1
#define COMMAND_FP_SUB 23
#define COMMAND_FP_MUL 24
#define COMMAND_FP_DIV 25
#define COMMAND_FP_POWER 26
#define COMMAND_FP_CEIL 27
#define COMMAND_FP_ROUND 28


// load N {u64 n times}
// save N {u64 n times} in (*sp)
// malloc size (return pointer on stack)
// free heap (pointer on stack)

// LOAD DATA ON STACK

// Perfect:
// MALLOC
// LOAD DATA BY POINTER

#define COMMAND_MALLOC 29 // *sp = sizeof heap -> returns pointer on stack: *sp = malloc(*sp)
#define COMMAND_LOAD 30 // uint64 N, N bytes
#define COMMAND_FREE 31 // *sp
//#define COMMAND_OMIT 32

struct vm_state {
	uint64_t* sp; // stack pointer
};

#endif

