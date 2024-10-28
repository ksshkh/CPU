#ifndef ASM_HPP
#define ASM_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../errors.hpp"
#include "../stack/stack.hpp"
#include "../commands.hpp"

struct Label {
    int address = 0;
    char* name = NULL;
};

struct Command {
    char *cmd = NULL;
    int cmd_code = 0;
    int argc = 0;
    int label = 0;
    Registers reg = REG_DEFAULT;
};

struct Assembler {
    const char* file_name_input = NULL;
    const char* file_name_output = NULL;

    char* buf_input = NULL;
    int* buf_output = NULL;

    size_t size_file = 0;
    size_t n_cmd = 0;
    size_t n_words = 0;

    Command* cmds = NULL;
    Label* lbls = NULL;
};

const int NUM_OF_LABELS = 20;

#define ASM_CTOR(asmblr) AsmCtor((asmblr), (argc), (argv), (code_error))

#define PROGRAM_INPUT(asmblr) ProgramInput((asmblr), (code_error))

#define ELEMENTS_COUNTER(asmblr) ElemetsCounter((asmblr), (code_error))

#define COMMANDS_PARCING(asmblr) CommandsParcing((asmblr), (code_error))

#define BUFFER_PARCING(asmblr) BufferParcing((asmblr), (code_error))

#define BUFFER_FILLING(command_struct, buf_output, buff_indx) BufferFilling((command_struct), (buf_output), (buff_indx), (code_error))

#define ARGUMENTS_PARCING(asmblr, i, argc) ArgumentsParcing((asmblr), (i), (argc), (code_error))

#define ARGUMENTS_HANDLING(asmblr, i, argc) ArgumentsHandling((asmblr), (i), (argc), (code_error))

#define OUTPUT(asmblr) Output((asmblr), (code_error))

#define LABEL_INSERT(cmd, asmblr, j) LabelInsert((cmd), (asmblr), (j), (code_error))

#define LABEL_FIND(cmd, asmblr, label_len) LabelFind((cmd), (asmblr), (label_len), (code_error))

#define ASM_DUMP(asmblr) AsmDump((asmblr), (code_error))

#define ASM_DTOR(asmblr) AsmDtor((asmblr), (code_error))

void ProgramInput(Assembler* asmblr, int* code_error);

void AsmCtor(Assembler* asmblr, int argc, char* argv[], int* code_error);

void ElemetsCounter(Assembler* asmblr, int* code_error);

void CommandsParcing(Assembler* asmblr, int* code_error);

void BufferParcing(Assembler* asmblr, int* code_error);

void BufferFilling(const Command command_struct, int* buf_output, int* buff_indx, int* code_error);

void ArgumentsParcing(Assembler* asmblr, size_t i, char* argc, int* code_error);

void ArgumentsHandling(Assembler* asmblr, size_t i, char* argc, int* code_error);

void Output(Assembler* asmblr, int* code_error);

void LabelInsert(char* cmd, Assembler* asmblr, int* j, int* code_error);

int LabelFind(char* cmd, Assembler* asmblr, size_t label_len, int* code_error);

void AsmDump(Assembler* asmblr, int* code_error);

void AsmDtor(Assembler* asmblr, int* code_error);

#endif // ASM_HPP
