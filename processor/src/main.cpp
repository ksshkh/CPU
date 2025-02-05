#include "../inc/processor.hpp"

int main(void) {

    int code_error = 0;

    SPU spu = {};

    SPUCtor(&spu, &code_error);
    SPURun (&spu, &code_error);
    SPUDump(&spu, &code_error);
    SPUDtor(&spu, &code_error);

    return 0;
}