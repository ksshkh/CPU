#include "disasm.hpp"

int main(void) {

    int code_error = 0;

    Disassembler disasmblr = {};

    DisasmCtor(&disasmblr, &code_error);
    DisasmRun (&disasmblr, &code_error);
    DisasmDtor(&disasmblr, &code_error);

    return 0;
}