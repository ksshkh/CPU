#ifndef ASM_HPP
#define ASM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../errors.hpp"

enum Commands {
    CMD_HLT = -1,
    CMD_DUMP,
    CMD_PUSH,
    CMD_POP,
    CMD_OUT,
    CMD_IN,
    CMD_ADD,
    CMD_SUB,
    CMD_MUL,
    CMD_DIV,
    CMD_SQRT,
    CMD_SIN,
    CMD_COS
};

struct Buffer {
    char* buffer = NULL;
    long int size_of_file = 0;
};

struct PartitionedBuffer {
    char** part_buffer = NULL;
    int size_of_part_buffer = 0;
};

long int count_size_file(FILE* program);

Errors BuffCtor(Buffer* buff);

Errors PartBuffCtor(PartitionedBuffer* prtbuff, const Buffer* buff);

int count_num_of_symbols(const Buffer* buff);

Errors AsmRun(const PartitionedBuffer* prtbuff);

#endif
