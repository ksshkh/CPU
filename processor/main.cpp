#include "processor.hpp"

int main(void) {
    Errors err = NO_ERROR;
    SPU spu = {};

    CHECKED_ SPUCtor(&spu);
    CHECKED_ SPURun(&spu);
    SPUDump(&spu);
    // STACK_DUMP(&spu.stk);
    return 0;
}