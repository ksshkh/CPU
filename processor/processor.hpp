#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../stack/stack.hpp"
#include "../errors.hpp"
#include "../assembler/asm.hpp"

const size_t InitCapacity = 5;

struct SPU {
    const char* file_name_input = NULL;

    size_t ip = 0;

    size_t code_size = 0;
    int* code = NULL;

    Stack_t stk = {};

    int registers[4] = {};
};

Errors SPUCtor(SPU* spu);

Errors CodeReader(SPU* spu);

Errors SPURun(SPU* spu);

void SPUDump(SPU* spu);

#endif