#include "asm.hpp"

int main(int argc, char* argv[]) {

    int code_error = 0;

    Assembler asmblr = {0};

    ASM_CTOR       (&asmblr, &code_error);
    BufferParcing  (&asmblr, &code_error);
    CommandsParcing(&asmblr, &code_error);
    Output         (&asmblr, &code_error);
    AsmDump        (&asmblr, &code_error);

    return 0;
}