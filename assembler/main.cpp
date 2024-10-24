#include "asm.hpp"

int main(void) {
    Assembler asmblr = {0};
    AsmCtor(&asmblr);
    // fprintf(stderr, "words: %d cmds: %d\n", asmblr.n_words, asmblr.n_cmd);
    CHECKED_ Parcing(&asmblr);
    CHECKED_ CommandsParcing(&asmblr);
    AsmDump(&asmblr);
    CHECKED_ Output(&asmblr);
    CHECKED_ AsmDtor(&asmblr);

    return 0;
}