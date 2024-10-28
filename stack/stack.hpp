#ifndef STACK_HPP
#define STACK_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../errors.hpp"

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG(code)
#endif

#define STACK_ASSERT(stk) {                                           \
    int err = STACK_VERIFICATION(stk);                                \
    if (err != NO_ERROR) {                                            \
        STACK_DUMP(stk);                                              \
    }                                                                 \
}

#define STACK_CTOR(stk, initCapacity) StackCtor((stk), (initCapacity), __FILE__, __func__, __LINE__, (code_error))

#define STACK_DTOR(stk) StackDtor((stk), (code_error))

#define STACK_DUMP(stk) StackDump((stk), __FILE__, __func__, __LINE__, (code_error))

#define STACK_PUSH(stk, el) StackPush((stk), (el), (code_error))

#define STACK_POP(stk, x) StackPop((stk), (x), (code_error))

#define STACK_VERIFICATION(stk) StackVerification((stk), (code_error))

#define STACK_REALLOCATION(stk, id) StackReallocation((stk), (id), (code_error))

typedef int StackElem_t;

typedef unsigned long long int Hash_t;

typedef unsigned long long int Canary_t;

const Canary_t STACK_CANARY = 0XACCE55ED;
const Canary_t DATA_CANARY = 0XC0FFEE;

const StackElem_t POISON = -666;

enum FunkId {
    PUSH_ID,
    POP_ID
};

struct Stack_t {

    const char* debug_file_name = NULL;

    ON_DEBUG(Canary_t left_canary = 0;)

    ON_DEBUG(Hash_t stack_hash = 0;)
    ON_DEBUG(Hash_t data_hash = 0;)

    ON_DEBUG(const char* file = NULL;)
    ON_DEBUG(const char* func = NULL;)
    ON_DEBUG(int line = 0;)

    StackElem_t* data = NULL;
    size_t position = 0;
    size_t capacity = 0;

    ON_DEBUG(Canary_t right_canary = 0;)
};

void StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line, int* code_error);

void StackDtor(Stack_t* stk, int* code_error);

void StackPush(Stack_t* stk, StackElem_t el, int* code_error);

void StackPop(Stack_t* stk, StackElem_t* x, int* code_error);

void StackDump(Stack_t* stk, const char* file, const char* func, int line, int* code_error);

int StackVerification(const Stack_t* stk, int* code_error);

void StackReallocation(Stack_t* stk, FunkId id, int* code_error);

void PoisonMaker(Stack_t* stk);

Hash_t DataHash(const Stack_t* stk);

Hash_t StackHash(const Stack_t* stk);

#endif // STACK_HPP