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

#define DISASM_CTOR(disasmblr) DisasmCtor((disasmblr), (code_error))

#define DISASM_RUN(disasmblr) DisasmRun((disasmblr), (code_error))

#define PRINT_ARGUMENT(buff, i, result) PrintArgument((buff), (i), (result), (code_error))

#define PRINT_REGS(reg, result) PrintRegs((reg), (result), (code_error))

#define DISASM_DTOR(disasmblr) DisasmDtor((disasmblr), (code_error))

void DisasmCtor(Disassembler* disasmblr, int* code_error);

void DisasmRun(Disassembler* disasmblr, int* code_error);

void PrintArgument(int* buff, size_t* i, FILE* result, int* code_error);

void PrintRegs(int reg, FILE* result, int* code_error);

void DisasmDtor(Disassembler* disasmblr, int* code_error);

#endif // DISASM_HPP