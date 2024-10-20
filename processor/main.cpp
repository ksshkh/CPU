#include "processor.hpp"

int main(void) {
    SPU spu = {};

    CHECKED_ SPUCtor(&spu);
    CHECKED_ SPURun(&spu);
    SPUDump(&spu);
    // STACK_DUMP(&spu.stk);
    CHECKED_ SpuDtor(&spu);
    return 0;
}
// 0011 0001
// 0000 0001