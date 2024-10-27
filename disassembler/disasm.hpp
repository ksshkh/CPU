#ifndef DISASM_HPP
#define DISASM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../errors.hpp"
#include "../stack/stack.hpp"
#include "../commands.hpp"

struct Disassembler {
    const char* file_name_input = NULL;
    const char* file_name_output = NULL;

    int* buf_input = NULL;

    size_t size_file = 0;
};

int DisasmCtor(Disassembler* disasmblr);

int DisasmRun(Disassembler* disasmblr);

int PrintArgument(int* buff, size_t* i, FILE* result);

int PrintRegs(int reg, FILE* result);

int DisasmDtor(Disassembler* disasmblr);

#endif // DISASM_HPP