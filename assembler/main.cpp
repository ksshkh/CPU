#include "asm.hpp"

int main(int argc, char* argv[]) {

    Assembler asmblr = {0};
    
    AsmCtor(&asmblr, argc, argv);
    CHECKED_ BufferParcing(&asmblr);
    CHECKED_ CommandsParcing(&asmblr);
    CHECKED_ BufferFilling(&asmblr);
    AsmDump(&asmblr);
    CHECKED_ Output(&asmblr);
    CHECKED_ AsmDtor(&asmblr);

    return 0;
}