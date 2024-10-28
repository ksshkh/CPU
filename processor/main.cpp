#include "processor.hpp"

int main(void) {

    int* code_error = (int*)calloc(1, sizeof(int));
    *code_error = 0;

    SPU spu = {};

    SPU_CTOR(&spu);
    SPU_RUN(&spu);
    SPU_DUMP(&spu);
    SPU_DTOR(&spu);

    return 0;
}