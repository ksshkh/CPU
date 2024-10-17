#include "./stack/stack.hpp"

static const char* errors_names[] = {"NO_ERROR",
                                     "PTR_ERROR",
                                     "SIZE_ERROR",
                                     "STACK_UNDERFLOW",
                                     "NO_STACK",
                                     "FILE_ERROR",
                                     "READ_ERR",
                                     "BAD_CAPACITY",
                                     "NO_DATA",
                                     "BAD_DATA_RIGHT_CANARY",
                                     "BAD_DATA_LEFT_CANARY",
                                     "BAD_STACK_RIGHT_CANARY",
                                     "BAD_STACK_LEFT_CANARY",
                                     "BAD_DATA_HASH",
                                     "BAD_STACK_HASH",
                                     "BAD_HASH",
                                     "BAD_DATA_CANARIES",
                                     "BAD_STACK_CANARIES"};

void my_strerr(int code_error, FILE* stream) {
    for (int i = 0; i < N_ERROR; i++)
    {
        if (code_error & (1 << i))
            fprintf(stream, "\x1b[31mERROR: %s\x1b[0m\n", errors_names[i]);
    }
}