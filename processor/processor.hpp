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
const size_t RAM_SIZE = 100;

const int check_mask = 15;

struct SPU {
    const char* file_name_input = NULL;

    size_t ip = 0;

    size_t code_size = 0;
    int* code = NULL;

    Stack_t stk = {};

    int* registers = NULL;
    int* ram = NULL;
};

int SPUCtor(SPU* spu);

int CodeReader(SPU* spu);

int SPURun(SPU* spu);

void SPUDump(SPU* spu);

int SpuDtor(SPU* spu);

#endif // PROCESSOR_HPP