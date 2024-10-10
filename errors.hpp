#ifndef ERRORS_HPP
#define ERRORS_HPP

#define ERR(str) "\x1b[31m" str "\x1b[0m\n"

enum Errors {
    NO_ERROR,
    PTR_ERROR,
    SIZE_ERROR,
    STACK_UNDERFLOW,
    NO_STACK,
    BAD_CAPACITY,
    NO_DATA,
    BAD_DATA_RIGHT_CANARY,
    BAD_DATA_LEFT_CANARY,
    BAD_STACK_RIGHT_CANARY,
    BAD_STACK_LEFT_CANARY,
    BAD_DATA_HASH,
    BAD_STACK_HASH,
    BAD_HASH,
    BAD_DATA_CANARIES,
    BAD_STACK_CANARIES
};

#define CHECKED_ if(!err) err =

#define MY_ASSERT(expression, err) if(!(expression)) {                                                                      \
    fprintf(stderr, ERR("%s: %d (%s) My assertion failed: \"" #expression "\""), __FILE__, __LINE__, __func__);             \
    return err;                                                                                                             \
}                                                                                                                           \

#endif // ERRORS_HPP