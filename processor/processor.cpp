#include "processor.hpp"

static const char* DEBUG_FILE_NAME = "../debug/processor_dump.txt";

void SPUCtor(SPU* spu, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    STACK_CTOR(&spu->stk,      InitCapacity, code_error);
    STACK_CTOR(&spu->func_stk, InitCapacity, code_error);

    spu->registers = (int*)calloc(REG_SIZE, sizeof(int));
    MY_ASSERT(spu->registers != NULL, PTR_ERROR);

    spu->ram = (int*)calloc(RAM_SIZE, sizeof(int));
    MY_ASSERT(spu->ram != NULL, PTR_ERROR);

    spu->file_name_input = "../programs/result.bin";

    CodeReader(spu, code_error);

}

void CodeReader(SPU* spu, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    FILE* program = fopen(spu->file_name_input, "rb");
    MY_ASSERT(program != NULL, FILE_ERROR);

    spu->code_size = COUNT_SIZE_FILE(program) / sizeof(int);

    spu->code = (int*)calloc(spu->code_size, sizeof(int));
    MY_ASSERT(spu->code != NULL, PTR_ERROR);

    fread(spu->code, sizeof(int), spu->code_size, program);

    MY_ASSERT(fclose(program) == 0,FILE_ERROR);

}

void SPURun(SPU* spu, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    spu->ip = 0;

    while(spu->ip < spu->code_size) {

        SPUDump(spu, code_error);

        int current_cmd = spu->code[spu->ip];

        if(current_cmd == CMD_HLT) {
            break;
        }
        else if(current_cmd == CMD_DUMP) {
            STACK_DUMP(&(spu->stk), code_error);
        }
        else if((current_cmd & CHECK_MASK) == CMD_PUSH) {
            StackElem_t* ptr = NULL;
            ptr = GetArgument(spu, current_cmd, code_error);

            MY_ASSERT(ptr != NULL, PTR_ERROR);

            StackPush(&(spu->stk), *ptr, code_error);

            if(!(spu->code[spu->ip] & MEM_MASK) && (spu->code[spu->ip] & REG_MASK) && (spu->code[spu->ip] & ARGC_MASK)) {
                spu->ip += 2;
                free(ptr);
            }
        }
        else if((current_cmd & CHECK_MASK) == CMD_POP) {
            StackElem_t* ptr = NULL;
            ptr = GetArgument(spu, current_cmd, code_error);

            if(ptr != NULL) {
                StackPop(&(spu->stk), ptr, code_error);
            }
            else {
                int pop_elem = 0;
                StackPop(&(spu->stk), &pop_elem, code_error);
            }
        }
        else if(current_cmd == CMD_OUT) {
            StackElem_t x = 0;
            StackPop(&(spu->stk), &x, code_error);
            fprintf(stderr, "%d\n", x);
        }
        else if(current_cmd == CMD_IN) {
            StackElem_t x = 0;
            fprintf(stderr, "enter a number to push pls\n");
            scanf("%d", &x);
            StackPush(&(spu->stk), x, code_error);
        }
        else if(current_cmd == CMD_ADD) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);
            StackPush(&(spu->stk), x1 + x2, code_error);
        }
        else if(current_cmd == CMD_SUB) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);
            StackPush(&(spu->stk), x1 - x2, code_error);
        }
        else if(current_cmd == CMD_MUL) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);
            StackPush(&(spu->stk), x1 * x2, code_error);
        }
        else if(current_cmd == CMD_DIV) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);
            StackPush(&(spu->stk), x1 / x2, code_error);
        }
        else if(current_cmd == CMD_SQRT) {
            StackElem_t x = 0;

            StackPop(&(spu->stk), &x, code_error);
            StackPush(&(spu->stk), (StackElem_t)sqrt(x), code_error);
        }
        else if(current_cmd == CMD_COS) {
            StackElem_t x = 0;

            StackPop(&(spu->stk), &x, code_error);
            StackPush(&(spu->stk), (StackElem_t)cos(x), code_error);
        }
        else if(current_cmd == CMD_SIN) {
            StackElem_t x = 0;

            StackPop(&(spu->stk), &x, code_error);
            StackPush(&(spu->stk), (StackElem_t)sin(x), code_error);
        }
        else if(current_cmd == CMD_JA) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 > x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JAE) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 >= x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JB) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 < x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JBE) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 <= x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JE) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 == x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JNE) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            StackPop(&(spu->stk), &x2, code_error);
            StackPop(&(spu->stk), &x1, code_error);

            if(x1 != x2) {
                spu->ip = spu->code[spu->ip + 1] - 1;
            }
            else {
                (spu->ip)++;
            }
        }
        else if(current_cmd == CMD_JMP) {
            spu->ip = spu->code[spu->ip + 1] - 1;
        }
        else if(current_cmd == CMD_CALL) {
            StackPush(&(spu->func_stk), StackElem_t(spu->ip + 2), code_error);
            spu->ip = spu->code[spu->ip + 1] - 1;
        }
        else if(current_cmd == CMD_RET) {
            if(spu->func_stk.position != 0) {
                int ret_address = 0;
                StackPop(&(spu->func_stk), &ret_address, code_error);
                spu->ip = ret_address - 1;
            }
        }
        else if(current_cmd == CMD_COUT) {
            StackElem_t x = 0;
            StackPop(&(spu->stk), &x, code_error);
            fprintf(stderr, "%c\n", x);
        }
        else if(current_cmd == CMD_DRAW) {
            StackElem_t line = 0;
            StackPop(&(spu->stk), &line, code_error);

            Draw(spu, line, code_error);
        }

        (spu->ip)++;
    }
}

StackElem_t* GetArgument(SPU* spu, int current_cmd, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    if ((current_cmd & REG_MASK) && (current_cmd & ARGC_MASK) && (current_cmd & MEM_MASK)) {
        return &spu->ram[(size_t) spu->registers[spu->code[++spu->ip] - 1] + spu->code[++spu->ip]];
    }
    else if ((current_cmd & MEM_MASK) && (current_cmd & REG_MASK)) {
        return &spu->ram[spu->registers[spu->code[++(spu->ip)] - 1]];
    }
    else if ((current_cmd & MEM_MASK) && (current_cmd & ARGC_MASK)) {
        return &spu->ram[spu->code[++(spu->ip)]];
    }
    else if ((current_cmd & REG_MASK) && (current_cmd & ARGC_MASK)) {
        StackElem_t *arg = (StackElem_t*)calloc(1, sizeof(StackElem_t));
        *arg = (StackElem_t)(spu->registers[spu->code[spu->ip + 1] - 1] + spu->code[spu->ip + 2]);
        return arg;
    }
    else if(current_cmd & REG_MASK) {
        return &spu->registers[spu->code[++(spu->ip)] - 1];
    }
    else if(current_cmd & ARGC_MASK) {
        return &spu->code[++(spu->ip)];
    }

    return NULL;
}

void Draw(SPU* spu, StackElem_t line, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    for(size_t i = 1; i <= (line * line); i++) {

        if(spu->ram[i - 1] != 0) {
            fprintf(stderr, "*");
        }
        else {
            fprintf(stderr, " ");
        }

        if(i % line == 0) {
            fprintf(stderr, "\n");
        }
    }
}

void SPUDump(SPU* spu, int* code_error) {

    FILE* debug = fopen(DEBUG_FILE_NAME, "a");

    if(debug != NULL) {
        if(*code_error) {
            fprintf(stderr, "code error %d\n", *code_error);
        }
        if(spu != NULL) {
            fprintf(debug, "size of code: %ld\n", spu->code_size);

            if(spu->code != NULL) {
                fprintf(debug, "code: \n");
                for(size_t i = 0; i < spu->code_size; i++) {
                    fprintf(debug, "%3ld|", i);
                }
                fprintf(debug, "\n");
                for(size_t i = 0; i < spu->code_size; i++) {
                    fprintf(debug, "----");
                }
                fprintf(debug, "\n");
                for(size_t i = 0; i < spu->code_size; i++) {
                    fprintf(debug, "%3d|", spu->code[i]);
                }
                fprintf(debug, "\n");
                for(size_t i = 0; i < spu->ip; i++) {
                    fprintf(debug, "    ");
                }
                fprintf(debug, "^\n");
                for(size_t i = 0; i < spu->ip; i++) {
                    fprintf(debug, "    ");
                }
                fprintf(debug, "ip = %ld\n", spu->ip);
                }
            else {
                fprintf(debug, "no code\n");
                fprintf(debug, "ip = %ld\n", spu->ip);
            }

            if(spu->registers != NULL) {
                fprintf(debug, "registers: ");
                for(size_t i = 0; i < REG_SIZE; i++) {
                    fprintf(debug, "%d ", spu->registers[i]);
                }
                fprintf(debug, "\n");
            }
            else {
                fprintf(debug, "no registers\n");
            }

            if(spu->ram != NULL) {
                fprintf(debug, "RAM:\n");
                for(size_t i = 1; i <= RAM_SIZE; i++) {
                    fprintf(debug, "%d ", spu->ram[i - 1]);
                    if(i % ram_line == 0) {
                        fprintf(debug, "\n");
                    }
                }
                fprintf(debug, "\n");
            }
            else {
                fprintf(debug, "no ram\n");
            }

            if(fclose(debug)) {
                fprintf(stderr, "file did not close\n");
            }

            STACK_DUMP(&(spu->stk), code_error);
            STACK_DUMP(&(spu->func_stk), code_error);
        }
        else {
            fprintf(debug, "no spu\n");
        }
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

void SPUDtor(SPU* spu, int* code_error) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    free(spu->code);
    free(spu->ram);
    free(spu->registers);

    spu->code = NULL;
    spu->ram = NULL;
    spu->registers = NULL;

    spu->file_name_input = NULL;

    spu->code_size = 0;
    spu->ip = 0;

    StackDtor(&(spu->stk), code_error);
    StackDtor(&(spu->func_stk), code_error);

}