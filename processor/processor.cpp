#include "processor.hpp"

int SPUCtor(SPU* spu) {
    MY_ASSERT(spu != NULL, PTR_ERROR);

    CHECKED_ STACK_CTOR(&spu->stk, InitCapacity);
    CHECKED_ STACK_CTOR(&spu->funcstk, InitCapacity);

    spu->file_name_input = "../assembler/result.bin";

    CHECKED_ CodeReader(spu);

    spu->registers = (int*)calloc(REG_SIZE, sizeof(int));
    MY_ASSERT(spu->registers != NULL, PTR_ERROR);

    spu->ram = (int*)calloc(RAM_SIZE, sizeof(int));
    MY_ASSERT(spu->ram != NULL, PTR_ERROR);

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

        else if((current_cmd & check_mask) == CMD_PUSH) {
            (spu->ip)++;

            if((current_cmd & MEM_MASK) == MEM_MASK) {

                if(((current_cmd & REG_MASK) == REG_MASK) && ((current_cmd & ARGC_MASK) == ARGC_MASK)) {
                    int temp_arg_reg = spu->registers[spu->code[spu->ip] - 1];
                    (spu->ip)++;
                    int temp_arg_argc = spu->code[spu->ip];
                    CHECKED_ StackPush(&(spu->stk), spu->ram[temp_arg_reg + temp_arg_argc]);
                }

                else if((current_cmd & REG_MASK) == REG_MASK) {
                    int temp_arg = spu->registers[spu->code[spu->ip] - 1];
                    CHECKED_ StackPush(&(spu->stk), spu->ram[temp_arg]);
                }

                else if((current_cmd & ARGC_MASK) == ARGC_MASK) {
                    CHECKED_ StackPush(&(spu->stk), spu->ram[spu->code[spu->ip]]);
                }

            }

            else {

                if(((current_cmd & REG_MASK) == REG_MASK) && ((current_cmd & ARGC_MASK) == ARGC_MASK)) {
                    int temp_arg_reg = spu->registers[spu->code[spu->ip] - 1];
                    (spu->ip)++;
                    int temp_arg_argc = spu->code[spu->ip];
                    CHECKED_ StackPush(&(spu->stk), temp_arg_reg + temp_arg_argc);
                }

                else if((current_cmd & REG_MASK) == REG_MASK) {
                    CHECKED_ StackPush(&(spu->stk), spu->registers[spu->code[spu->ip] - 1]);
                }

                else if((current_cmd & ARGC_MASK) == ARGC_MASK) {
                    CHECKED_ StackPush(&(spu->stk), spu->code[spu->ip]);
                }

            }
        }

        else if((current_cmd & check_mask) == CMD_POP) {

            if(current_cmd == CMD_POP) {
                int pop_elem = 0;
                CHECKED_ StackPop(&(spu->stk), &pop_elem);

            }

            else if((current_cmd & MEM_MASK) == MEM_MASK) {
                (spu->ip)++;

                if(((current_cmd & REG_MASK) == REG_MASK) && ((current_cmd & ARGC_MASK) == ARGC_MASK)) {
                    int temp_arg_reg = spu->registers[spu->code[spu->ip] - 1];
                    (spu->ip)++;
                    int temp_arg_argc = spu->code[spu->ip];
                    CHECKED_ StackPop(&(spu->stk), &(spu->ram[temp_arg_reg + temp_arg_argc]));
                }

                else if((current_cmd & REG_MASK) == REG_MASK) {
                    int temp_arg = spu->registers[spu->code[spu->ip] - 1];
                    CHECKED_ StackPop(&(spu->stk), &(spu->ram[temp_arg]));
                }

                else if((current_cmd & ARGC_MASK) == ARGC_MASK) {
                    CHECKED_ StackPop(&(spu->stk), &(spu->ram[spu->code[spu->ip]]));
                }

            }

            else if((current_cmd & REG_MASK) == REG_MASK) {
                (spu->ip)++;
                CHECKED_ StackPop(&(spu->stk), &(spu->registers[spu->code[spu->ip] - 1]));
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

        else if(current_cmd == CALL) {
            CHECKED_ StackPush(&(spu->funcstk), spu->ip + 2);
            spu->ip = spu->code[spu->ip + 1] - 1;
        }

        else if(current_cmd == RET) {
            int ret_address = 0;
            CHECKED_ StackPop(&(spu->funcstk), &ret_address);
            spu->ip = ret_address - 1;
        }

        (spu->ip)++;
    }
    return code_error;
}

void SPUDump(SPU* spu) {
    FILE* debug = fopen("../debug/dump.txt", "a");
    fprintf(debug, "size of code: %ld\n", spu->code_size);
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
    fprintf(debug, "registers: ");
    for(size_t i = 0; i < REG_SIZE; i++) {
        fprintf(debug, "%d ", spu->registers[i]);
    }
    fprintf(debug, "\n");
    fprintf(debug, "RAM: ");
    for(size_t i = 0; i < RAM_SIZE; i++) {
        fprintf(debug, "%d ", spu->ram[i]);
    }
    fprintf(debug, "\n");
    fprintf(debug, "Stack:\n");
    STACK_DUMP(&(spu->stk));
    fprintf(debug, "Function stack:\n");
    STACK_DUMP(&(spu->funcstk));

    fclose(debug);
}

int SpuDtor(SPU* spu) {
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
    return code_error;
}