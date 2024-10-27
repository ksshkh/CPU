#include "stack.hpp"

static const char * DEBUG_FILE_NAME = "../debug/processor_dump.txt";

int StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line) {

    MY_ASSERT(stk != NULL, PTR_ERROR);
    MY_ASSERT(initCapacity != 0, SIZE_ERROR);

    ON_DEBUG(stk->file = file;)
    ON_DEBUG(stk->func = func;)
    ON_DEBUG(stk->line = line;)

    stk->debug_file_name = DEBUG_FILE_NAME;

#ifdef DEBUG

    stk->data = (StackElem_t*)calloc(initCapacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t), sizeof(char));
    MY_ASSERT (stk->data != NULL, PTR_ERROR)

    *((Canary_t*)stk->data) = DATA_CANARY;
    stk->data = (StackElem_t*)((char*) stk->data + sizeof(Canary_t));
    *((Canary_t*)(stk->data + initCapacity)) = DATA_CANARY;

    stk->left_canary = STACK_CANARY;
    stk->right_canary = STACK_CANARY;

#else

    stk->data = (StackElem_t*)calloc(initCapacity * sizeof(StackElem_t), sizeof(StackElem_t));
    MY_ASSERT(stk->data != NULL, NO_DATA);

#endif

    stk->capacity = initCapacity;
    stk->position = 0;

    PoisonMaker(stk);

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return code_error;
}

int StackDtor(Stack_t* stk) {
    STACK_ASSERT(stk);

    ON_DEBUG(stk->data = (StackElem_t *) ((char *) stk->data - sizeof(Canary_t));)
    free(stk->data);
    stk->data = NULL;

    stk->capacity = 0;
    stk->position = 0;

    #ifdef DEBUG
    stk->data_hash = 0;
    stk->file = 0;
    stk->func = 0;
    stk->line = 0;
    stk->left_canary = 0;
    stk->right_canary = 0;
    stk->stack_hash = 0;
    stk->debug_file_name = NULL;
    #endif

    return code_error;
}

int StackPush(Stack_t* stk, StackElem_t el) {
    STACK_ASSERT(stk);

    if(stk->position == stk->capacity) {
        StackReallocation(stk, PUSH_ID);
    }

    stk->data[stk->position] = el;
    stk->position++;

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return code_error;
}

int StackPop(Stack_t* stk, StackElem_t* x) {
    MY_ASSERT(x != NULL, PTR_ERROR);
    STACK_ASSERT(stk);
    MY_ASSERT(stk->position != 0, STACK_UNDERFLOW);

    if(stk->capacity > (stk->position - 1) * 4 && stk->position != 1) {
        StackReallocation(stk, POP_ID);
    }

    stk->position--;
    *x = stk->data[stk->position];
    stk->data[stk->position] = POISON;

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return code_error;
}

void StackDump(Stack_t* stk, const char* file, const char* func, int line) {

    FILE* debug_file = fopen(stk->debug_file_name, "a");

    if(debug_file != NULL) {

        fprintf(debug_file, "------------------------------------\n");
        fprintf(debug_file, "called from %s: %d (%s)\n", file, line, func);

        my_strerr(code_error, debug_file);

        if(code_error) {
            fprintf(stderr, "code error %d\n", code_error);
        }

        fprintf(debug_file, "Stack_t [%p]\n", stk);
        fprintf(debug_file, "{\n");

        if(stk != NULL) {

            ON_DEBUG(fprintf(debug_file, "\tstack born at %s: %d (%s)\n", stk->file, stk->line, stk->func);)

            ON_DEBUG(fprintf(debug_file, "\tstack hash: %#llx\n", stk->stack_hash);)
            ON_DEBUG(fprintf(debug_file, "\tdata hash: %#llx\n", stk->data_hash);)

            ON_DEBUG(fprintf(debug_file, "\tstack left canary: %#llx\n", stk->left_canary);)

            fprintf(debug_file, "\tcapacity = %lu\n", stk->capacity);
            fprintf(debug_file, "\tposition = %lu\n", stk->position);
            fprintf(debug_file, "\tdata [%p]\n", stk->data);

            if(stk->data != NULL) {
                fprintf(debug_file, "\t{\n");

                ON_DEBUG(fprintf(debug_file, "\t\tdata left canary: %#llx\n", *((Canary_t*) stk->data - 1));)

                if(stk->capacity != 0) {
                    for(size_t i = 0; i < stk->capacity; i++) {
                        if(i < stk->position) {
                            fprintf(debug_file, "\t\t*[%lu] = %d\n", i, stk->data[i]);
                        }
                        else {
                            fprintf(debug_file, "\t\t[%lu] = %.d(POISON)\n", i, stk->data[i]);
                        }
                    }
                }
                else if(stk->position != 0) {
                    for(size_t i = 0; i < stk->position; i++) {
                        fprintf(debug_file, "\t\t*[%lu] = %d\n", i, stk->data[i]);
                    }
                }

                ON_DEBUG(fprintf(debug_file, "\t\tdata right canary: %#llx\n", *((Canary_t*)(stk->data + stk->capacity)));)
                fprintf(debug_file, "\t}\n");
            }

            ON_DEBUG(fprintf(debug_file, "\tstack right canary: %#llx\n", stk->right_canary);)
        }

        fprintf(debug_file, "}\n");
        fprintf(debug_file, "------------------------------------\n\n\n");

        if(fclose(debug_file)) {
            fprintf(stderr, "file did not close\n");
        }
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

int StackVerification(const Stack_t* stk) {
    if(!stk) {
        code_error |= NO_STACK;
        return code_error;
    }

    if(!stk->capacity) {
        code_error |= BAD_CAPACITY;
    }

    if(stk->capacity < stk->position) {
        code_error |= SIZE_ERROR;
    }
#ifdef DEBUG

    if(stk->left_canary != STACK_CANARY && stk->right_canary != STACK_CANARY) {
        code_error |= BAD_STACK_CANARIES;
    }

    if(stk->left_canary != STACK_CANARY) {
        code_error |= BAD_STACK_LEFT_CANARY;
    }

    if(stk->right_canary != STACK_CANARY) {
        code_error |= BAD_STACK_RIGHT_CANARY;
    }

#endif

    if(!stk->data) {
        code_error |= NO_DATA;
        return code_error;
    }

#ifdef DEBUG

    if(stk->data_hash != DataHash(stk) && stk->stack_hash != StackHash(stk)) {
        code_error |= BAD_HASH;
    }

    if(stk->data_hash != DataHash(stk)) {
        code_error |= BAD_DATA_HASH;
    }

    if(stk->stack_hash != StackHash(stk)) {
        code_error |= BAD_STACK_HASH;
    }

    if(*((Canary_t *) stk->data - 1) != DATA_CANARY && *((Canary_t *)(stk->data + stk->capacity)) != DATA_CANARY) {
        code_error |= BAD_DATA_CANARIES;
    }

    if(*((Canary_t *) stk->data - 1) != DATA_CANARY) {
        code_error |= BAD_DATA_LEFT_CANARY;
    }

    if(*((Canary_t *)(stk->data + stk->capacity)) != DATA_CANARY) {
        code_error |= BAD_DATA_RIGHT_CANARY;
    }

#endif

    return code_error;
}

int StackReallocation(Stack_t* stk, FunkId id) {
    STACK_ASSERT(stk);

    if(id == PUSH_ID) {
        stk->capacity *= 2;

#ifdef DEBUG

        stk->data = (StackElem_t*)((char*)stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
        MY_ASSERT(stk->data != NULL, NO_DATA);
        stk->data = (StackElem_t*)((char*)stk->data + sizeof(Canary_t));
        *((Canary_t*)(stk->data + stk->capacity)) = DATA_CANARY;

#else

        stk->data = (StackElem_t*) realloc(stk->data, sizeof(StackElem_t) * stk->capacity);
        MY_ASSERT(stk->data != NULL, NO_DATA);

#endif
    }
    else if(id == POP_ID) {
        stk->capacity /= 2;

        #ifdef DEBUG

        stk->data = (StackElem_t*)((char*)stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
        MY_ASSERT(stk->data != NULL, NO_DATA);
        stk->data = (StackElem_t*)((char*)stk->data + sizeof(Canary_t));
        *((Canary_t*)(stk->data + stk->capacity)) = DATA_CANARY;

        #else

        stk->data = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * stk->capacity);
        MY_ASSERT(stk->data != NULL, NO_DATA);

        #endif
    }

    PoisonMaker(stk);

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return code_error;
}

void PoisonMaker(Stack_t* stk) {
    for(size_t i = stk->position; i < stk->capacity; i++) {
        stk->data[i] = POISON;
    }
}

Hash_t DataHash(const Stack_t* stk) {
    Hash_t hash = 0, high = 0;
    for (size_t i = 0; i < stk->position; i++) {
        hash = (hash << 4) + (int)stk->data[i];
        if (high = hash & 0xF0000000) {
            hash ^= high >> 24;
        }
        hash &= ~high;
    }
    return hash;
}

Hash_t StackHash(const Stack_t* stk) {
    Hash_t hash = 0;
    StackElem_t data_sum = 0;
    for (size_t i = 0; i < stk->position; i++) {
        data_sum += stk->data[i];
    }

    hash += (int)data_sum;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += stk->capacity;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += stk->position;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}
