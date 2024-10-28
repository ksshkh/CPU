#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    StackElem_t x = 0;

    int* code_error = (int*)calloc(1, sizeof(int));
    *code_error = 0;

    STACK_CTOR(&stk, 5);

    // for(int i = 0; i < 10; i++) {
    //     CHECKED_ StackPush(&stk, i * 10);
    //     STACK_DUMP(&stk);
    // }
    STACK_PUSH(&stk, 3);
    STACK_PUSH(&stk, 78);
    STACK_POP(&stk, &x);

    // stk.data = NULL;

    for(int i = 0; i < 10; i++) {
        STACK_PUSH(&stk, 7);
    }

    STACK_DUMP(&stk);

    STACK_DTOR(&stk);

    return 0;
}