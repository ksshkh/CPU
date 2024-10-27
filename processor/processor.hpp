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
const size_t RAM_SIZE = 49;
const size_t ram_line = (size_t)(sqrt(RAM_SIZE));

const int CHECK_MASK = 15;

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

int SPUCtor(SPU* spu);

int CodeReader(SPU* spu);

int SPURun(SPU* spu);

int GetArgument(SPU* spu, int current_cmd, int* argument);

void SPUDump(SPU* spu);

int SpuDtor(SPU* spu);

#endif // PROCESSOR_HPP