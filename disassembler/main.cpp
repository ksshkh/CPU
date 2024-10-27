#include "disasm.hpp"

int main(void) {

    Disassembler disasmblr = {};

    CHECKED_ DisasmCtor(&disasmblr);
    CHECKED_ DisasmRun(&disasmblr);
    CHECKED_ DisasmDtor(&disasmblr);

    return 0;
}