#include "asm.hpp"

int main(int argc, char* argv[]) {

    int* code_error = (int*)calloc(1, sizeof(int));
    *code_error = 0;

    Assembler asmblr = {0};

    ASM_CTOR(&asmblr);
    BUFFER_PARCING(&asmblr);
    COMMANDS_PARCING(&asmblr);
    ASM_DUMP(&asmblr);
    OUTPUT(&asmblr);
    ASM_DUMP(&asmblr);

    return 0;
}