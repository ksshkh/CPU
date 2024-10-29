#include "stack.hpp"

int main(void) {

    Stack_t stk    = {};
    StackElem_t x  = 0;
    int code_error = 0;

    STACK_CTOR(&stk, 5, &code_error);

    StackPush(&stk,  3, &code_error);
    StackPush(&stk, 78, &code_error);
    StackPop (&stk, &x, &code_error);

    // stk.data = NULL;

    for(int i = 0; i < 10; i++) {
        StackPush(&stk, 7, &code_error);
    }

    STACK_DUMP(&stk, &code_error);

    StackDtor(&stk, &code_error);

    return 0;
}