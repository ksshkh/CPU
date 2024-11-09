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
    char* name  = NULL;
};

struct Command {
    char *cmd = NULL;

    int cmd_code = 0;

    int argc     = 0;
    int label    = 0;      

    Registers reg = REG_DEFAULT;
};

struct Assembler {
    const char* file_name_input  = NULL;
    const char* file_name_output = NULL;

    char* buf_input  = NULL;
    int*  buf_output = NULL;

    size_t size_file = 0;
    size_t n_cmd     = 0;
    size_t n_words   = 0;

    Command* cmds = NULL;
    Label* lbls   = NULL;
};

const int NUM_OF_LABELS = 20;
const int NO_ARGUMENT = 0xDEAD;

void ProgramInput(Assembler* asmblr, int* code_error);

void AsmCtor(Assembler* asmblr, int argc, char* argv[], int* code_error);

void ElementsCounter(Assembler* asmblr, int* code_error);

void CommandsParcing(Assembler* asmblr, int* code_error);

void BufferParcing(Assembler* asmblr, int* code_error);

void BufferFilling(const Command command_struct, int* buf_output, int* buff_indx, int* code_error);

void ArgumentsParcing(Command* command_struct, Assembler* asmblr, char* argc, int* code_error);

void GetArg(Command* command_struct, Assembler* asmblr, char* argc, int* code_error);

Registers RegParcing(Command* command_struct, char* argc, int* code_error);

StackElem_t ValueParcing(Command* command_struct, char* argc, int* code_error);

void Output(Assembler* asmblr, int* code_error);

void LabelInsert(char* cmd, Assembler* asmblr, int* j, int* code_error);

int LabelFind(char* cmd, Assembler* asmblr, size_t label_len, int* code_error);

void AsmDump(Assembler* asmblr, int* code_error);

void AsmDtor(Assembler* asmblr, int* code_error);

#endif // ASM_HPP
