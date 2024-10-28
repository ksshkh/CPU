#include "disasm.hpp"

int main(void) {

    int* code_error = (int*)calloc(1, sizeof(int));
    *code_error = 0;

    Disassembler disasmblr = {};

    DISASM_CTOR(&disasmblr);
    DISASM_RUN(&disasmblr);
    DISASM_DTOR(&disasmblr);

    return 0;
}