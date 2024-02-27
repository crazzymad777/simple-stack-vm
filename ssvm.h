#ifndef SIMPLESTACKVM_H
#define SIMPLESTACKVM_H

#include <inttypes.h>

#define COMMAND_PUSH 1 // uint64
#define COMMAND_POP 2 // uint64*
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
#define COMMAND_CALL 14
#define COMMAND_RET 15

// Doubt
#define COMMAND_DLOPEN 16
#define COMMAND_DLERROR 17
#define COMMAND_DLSYM 18
#define COMMAND_DLCLOSE 18

// sysv-amd64 call

// float-pointing values


// load N {u64 n times}
// save N {u64 n times} in (*sp)
// malloc size (return pointer on stack)
// free heap (pointer on stack)

// LOAD DATA ON STACK
// MALLOC
// COPY DATA FROM STACK ON POINTER

// Perfect:
// MALLOC
// LOAD DATA BY POINTER

struct vm_state {
	uint64_t* sp; // stack pointer
};

#endif

