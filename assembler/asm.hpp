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
    const char* file_name_print_txt = NULL;

    char* buf_input = NULL;
    int* buf_output = NULL;

    size_t size_file = 0;
    size_t n_cmd = 0;
    size_t n_words = 0;

    Command* cmds = NULL;
    Label* lbls = NULL;
};

const int NUM_OF_LABELS = 20;

int ProgramInput(Assembler* asmblr);

int AsmCtor(Assembler* asmblr, int argc, char* argv[]);

int ElemetsCounter(Assembler* asmblr);

int CommandsParcing(Assembler* asmblr);

int BufferParcing(Assembler* asmblr);

int BufferFilling(Assembler* asmblr);

int ArgumentsParcing(Assembler* asmblr, size_t i, char* argc);

int ArgumentsHandling(Assembler* asmblr, size_t i, char* argc);

int Output(Assembler* asmblr);

int LabelInsert(char* cmd, Assembler* asmblr, int* j);

int LabelFind(char* cmd, Assembler* asmblr);

void AsmDump(Assembler* asmblr);

int AsmDtor(Assembler* asmblr);

#endif // ASM_HPP
