#include "processor.hpp"

int main(void) {
    
    SPU spu = {};

    CHECKED_ SPUCtor(&spu);
    CHECKED_ SPURun(&spu);
    SPUDump(&spu);
    CHECKED_ SpuDtor(&spu);

    return 0;
}