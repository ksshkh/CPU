#include "processor.hpp"

int SPUCtor(SPU* spu) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    CHECKED_ STACK_CTOR(&spu->stk, InitCapacity);
    CHECKED_ STACK_CTOR(&spu->func_stk, InitCapacity);

    spu->registers = (int*)calloc(REG_SIZE, sizeof(int));
    MY_ASSERT(spu->registers != NULL, PTR_ERROR);

    spu->ram = (int*)calloc(RAM_SIZE, sizeof(int));
    MY_ASSERT(spu->ram != NULL, PTR_ERROR);

    spu->file_name_input = "../programs/result.bin";

    CHECKED_ CodeReader(spu);

    return code_error;
}

int CodeReader(SPU* spu) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    FILE* program = fopen(spu->file_name_input, "rb");
    MY_ASSERT(program != NULL, FILE_ERROR);

    spu->code_size = count_size_file(program) / sizeof(int);

    spu->code = (int*)calloc(spu->code_size, sizeof(int));
    MY_ASSERT(spu->code != NULL, PTR_ERROR);

    fread(spu->code, sizeof(int), spu->code_size, program);

    MY_ASSERT(fclose(program) == 0,FILE_ERROR);

    return code_error;
}

int SPURun(SPU* spu) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    spu->ip = 0;

    while(spu->ip < spu->code_size) {
        SPUDump(spu);

        int current_cmd = spu->code[spu->ip];

        if(current_cmd == CMD_HLT) {
            break;
        }
        else if(current_cmd == CMD_DUMP) {
            STACK_DUMP(&(spu->stk));
        }
        else if((current_cmd & CHECK_MASK) == CMD_PUSH) {
            (spu->ip)++;

            int argument = 0;
            CHECKED_ GetArgument(spu, current_cmd, &argument);

            if(current_cmd & MEM_MASK) {
                CHECKED_ StackPush(&(spu->stk), spu->ram[argument]);
            }
            else {
                CHECKED_ StackPush(&(spu->stk), argument);
            }
        }
        else if((current_cmd & CHECK_MASK) == CMD_POP) {

            if(current_cmd == CMD_POP) {
                int pop_elem = 0;
                CHECKED_ StackPop(&(spu->stk), &pop_elem);
                (spu->ip)++;
                continue;
            }

            (spu->ip)++;
            int argument = 0;
            CHECKED_ GetArgument(spu, current_cmd, &argument);

            if(current_cmd & MEM_MASK) {
                CHECKED_ StackPop(&(spu->stk), &(spu->ram[argument]));
            }
            else if(current_cmd & REG_MASK) {
                CHECKED_ StackPop(&(spu->stk), &(spu->registers[argument]));
            }
        }
        else if(current_cmd == CMD_OUT) {
            StackElem_t x = 0;
            CHECKED_ StackPop(&(spu->stk), &x);
            fprintf(stderr, "%d\n", x);
        }
        else if(current_cmd == CMD_IN) {
            StackElem_t x = 0;
            fprintf(stderr, "enter a number to push pls\n");
            scanf("%d", &x);
            CHECKED_ StackPush(&(spu->stk), x);
        }
        else if(current_cmd == CMD_ADD) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);
            CHECKED_ StackPush(&(spu->stk), x1 + x2);
        }
        else if(current_cmd == CMD_SUB) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);
            CHECKED_ StackPush(&(spu->stk), x1 - x2);
        }
        else if(current_cmd == CMD_MUL) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);
            CHECKED_ StackPush(&(spu->stk), x1 * x2);
        }
        else if(current_cmd == CMD_DIV) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);
            CHECKED_ StackPush(&(spu->stk), x1 / x2);
        }
        else if(current_cmd == CMD_SQRT) {
            StackElem_t x = 0;

            CHECKED_ StackPop(&(spu->stk), &x);
            CHECKED_ StackPush(&(spu->stk), (StackElem_t)sqrt(x));
        }
        else if(current_cmd == CMD_COS) {
            StackElem_t x = 0;

            CHECKED_ StackPop(&(spu->stk), &x);
            CHECKED_ StackPush(&(spu->stk), (StackElem_t)cos(x));
        }
        else if(current_cmd == CMD_SIN) {
            StackElem_t x = 0;

            CHECKED_ StackPop(&(spu->stk), &x);
            CHECKED_ StackPush(&(spu->stk), (StackElem_t)sin(x));
        }
        else if(current_cmd == CMD_JA) {
            StackElem_t x1 = 0;
            StackElem_t x2 = 0;

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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

            CHECKED_ StackPop(&(spu->stk), &x2);
            CHECKED_ StackPop(&(spu->stk), &x1);

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
            CHECKED_ StackPush(&(spu->func_stk), StackElem_t(spu->ip + 2));
            spu->ip = spu->code[spu->ip + 1] - 1;
        }
        else if(current_cmd == CMD_RET) {
            if(spu->func_stk.position != 0) {
                int ret_address = 0;
                CHECKED_ StackPop(&(spu->func_stk), &ret_address);
                spu->ip = ret_address - 1;
            }
        }
        else if(current_cmd == CMD_COUT) {
            StackElem_t x = 0;
            CHECKED_ StackPop(&(spu->stk), &x);
            fprintf(stderr, "%c\n", x);
        }
        else if(current_cmd == CMD_DRAW) {
            for(size_t i = 1; i <= RAM_SIZE; i++) {
                if(spu->ram[i - 1] != 0) {
                    fprintf(stderr, "*");
                }
                else {
                    fprintf(stderr, " ");
                }

                if(i % ram_line == 0) {
                    fprintf(stderr, "\n");
                }
            }
        }

        (spu->ip)++;
    }

    return code_error;
}

int GetArgument(SPU* spu, int current_cmd, int* argument) {

    MY_ASSERT(spu != NULL, PTR_ERROR);

    if((current_cmd & REG_MASK) && (current_cmd & ARGC_MASK)) {
        int temp_arg_reg = spu->registers[spu->code[spu->ip] - 1];
        (spu->ip)++;
        int temp_arg_argc = spu->code[spu->ip];
        *argument = temp_arg_reg + temp_arg_argc;
    }
    else if(current_cmd & REG_MASK) {
        if((current_cmd & CHECK_MASK) == CMD_POP) {
            *argument = spu->code[spu->ip] - 1;
            return code_error;
        }

        int temp_arg = spu->registers[spu->code[spu->ip] - 1];
        *argument = temp_arg;
    }
    else if(current_cmd & ARGC_MASK) {
        *argument = spu->code[spu->ip];
    }

    return code_error;
}

void SPUDump(SPU* spu) {

    FILE* debug = fopen("../debug/processor_dump.txt", "a");
    if(debug != NULL) {
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

            fprintf(debug, "Stack:\n");
            STACK_DUMP(&(spu->stk));
            fprintf(debug, "Function stack:\n");
            STACK_DUMP(&(spu->func_stk));
        }
        else {
            fprintf(debug, "no spu\n");
        }

        if(fclose(debug)) {
            fprintf(stderr, "file did not close\n");
        }
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

int SpuDtor(SPU* spu) {

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

    CHECKED_ StackDtor(&(spu->stk));
    CHECKED_ StackDtor(&(spu->func_stk));

    return code_error;
}