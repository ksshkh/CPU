#include "asm.hpp"

int main(void) {
    Errors err = NO_ERROR;
    Assembler asmblr = {0};
    AsmCtor(&asmblr);
    printf("%ld\n", asmblr.n_cmd);
    printf("%ld\n", asmblr.size_file);
    CHECKED_ Parcing(&asmblr);

    for(size_t i = 0; i < asmblr.n_cmd; i++) {
        printf("%s\n", asmblr.cmd[i].cmd);
    }

    CHECKED_ CommandsParcing(&asmblr);
    for(size_t i = 0; i < asmblr.n_words; i++) {
        printf("%d ", asmblr.buf_output[i]);
    }
    printf("\n");
    for(size_t i = 0; i < asmblr.n_cmd; i++) {
        printf("%d %d %d %d\n", asmblr.cmd[i].cmd_code, asmblr.cmd[i].argc, asmblr.cmd[i].reg, asmblr.cmd[i].label);
    }
    printf("%ld\n", asmblr.n_cmd);
    Output(&asmblr);

    return 0;
}