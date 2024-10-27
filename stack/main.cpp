#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    StackElem_t x = 0;

    CHECKED_ STACK_CTOR(&stk, 5);

    // for(int i = 0; i < 10; i++) {
    //     CHECKED_ StackPush(&stk, i * 10);
    //     STACK_DUMP(&stk);
    // }
    CHECKED_ StackPush(&stk, 3);
    CHECKED_ StackPush(&stk, 78);
    CHECKED_ StackPop(&stk, &x);

    // stk.data = NULL;

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPush(&stk, 7);
    }

    CHECKED_ StackDtor(&stk);

    return 0;
}