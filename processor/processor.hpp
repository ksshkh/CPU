#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../stack/stack.hpp"
#include "../errors.hpp"
#include "../commands.hpp"

const size_t InitCapacity = 5;
const size_t REG_SIZE = 4;
const size_t RAM_SIZE = 10000;
const size_t ram_line = 10;

struct SPU {
    const char* file_name_input = NULL;

    size_t ip = 0;

    size_t code_size = 0;
    int* code = NULL;

    Stack_t stk = {};
    Stack_t func_stk = {};

    int* registers = NULL;
    int* ram = NULL;
};

#define SPU_CTOR(spu) SPUCtor((spu), (code_error))

#define CODE_READER(spu) CodeReader((spu), (code_error))

#define SPU_RUN(spu) SPURun((spu), (code_error))

#define GET_ARGUMENT(spu, current_cmd, argument) GetArgument((spu), (current_cmd), (argument), (code_error))

#define SPU_DUMP(spu) SPUDump((spu), (code_error))

#define SPU_DTOR(spu) SPUDtor((spu), (code_error))

void SPUCtor(SPU* spu, int* code_error);

void CodeReader(SPU* spu, int* code_error);

void SPURun(SPU* spu, int* code_error);

void GetArgument(SPU* spu, int current_cmd, int* argument, int* code_error);

void SPUDump(SPU* spu, int* code_error);

void SPUDtor(SPU* spu, int* code_error);

#endif // PROCESSOR_HPP