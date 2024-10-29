#include "disasm.hpp"

void DisasmCtor(Disassembler* disasmblr, int* code_error) {

    MY_ASSERT(disasmblr != NULL, PTR_ERROR);

    disasmblr->file_name_input = "../programs/result.bin";
    disasmblr->file_name_output = "../programs/retranslation.txt";

    FILE* program = fopen(disasmblr->file_name_input, "rb");
    MY_ASSERT(program != NULL, FILE_ERROR);

    disasmblr->size_file = COUNT_SIZE_FILE(program) / sizeof(int);

    disasmblr->buf_input = (int*)calloc(disasmblr->size_file, sizeof(int));
    MY_ASSERT(disasmblr->buf_input != NULL, PTR_ERROR);

    fread(disasmblr->buf_input, sizeof(int), disasmblr->size_file, program);

    MY_ASSERT(fclose(program) == 0, FILE_ERROR);

}

void DisasmRun(Disassembler* disasmblr, int* code_error) {

    MY_ASSERT(disasmblr != NULL, PTR_ERROR);

    FILE* result = fopen(disasmblr->file_name_output, "a");
    MY_ASSERT(result != NULL, FILE_ERROR);

    for(size_t i = 0; i < disasmblr->size_file; i++) {

        if(disasmblr->buf_input[i] == CMD_HLT) {
            fprintf(result, "hlt\n");
        }
        else if(disasmblr->buf_input[i] == CMD_DUMP) {
            fprintf(result, "dump\n");
        }
        else if((disasmblr->buf_input[i] & CHECK_MASK) == CMD_PUSH) {
            fprintf(result, "push ");

            if(disasmblr->buf_input[i] & MEM_MASK) {
                fprintf(result, "[");
                PrintArgument(disasmblr->buf_input, &i, result, code_error);
                fprintf(result, "]\n");
                continue;
            }

            PrintArgument(disasmblr->buf_input, &i, result, code_error);
            fprintf(result, "\n");
        }
        else if((disasmblr->buf_input[i] & CHECK_MASK) == CMD_POP) {
            if(disasmblr->buf_input[i] == CMD_POP) {
                fprintf(result, "pop\n");
                continue;
            }

            fprintf(result, "pop ");
            if(disasmblr->buf_input[i] & MEM_MASK) {
                fprintf(result, "[");
                PrintArgument(disasmblr->buf_input, &i, result, code_error);
                fprintf(result, "]\n");
                continue;
            }

            PrintArgument(disasmblr->buf_input, &i, result, code_error);
            fprintf(result, "\n");
        }
        else if(disasmblr->buf_input[i] == CMD_OUT) {
            fprintf(result, "out\n");
        }
        else if(disasmblr->buf_input[i] == CMD_IN) {
            fprintf(result, "in\n");
        }
        else if(disasmblr->buf_input[i] == CMD_ADD) {
            fprintf(result, "add\n");
        }
        else if(disasmblr->buf_input[i] == CMD_SUB) {
            fprintf(result, "sub\n");
        }
        else if(disasmblr->buf_input[i] == CMD_MUL) {
            fprintf(result, "mul\n");
        }
        else if(disasmblr->buf_input[i] == CMD_DIV) {
            fprintf(result, "div\n");
        }
        else if(disasmblr->buf_input[i] == CMD_SQRT) {
            fprintf(result, "sqrt\n");
        }
        else if(disasmblr->buf_input[i] == CMD_SIN) {
            fprintf(result, "sin\n");
        }
        else if(disasmblr->buf_input[i] == CMD_COS) {
            fprintf(result, "cos\n");
        }
        else if(disasmblr->buf_input[i] == CMD_JA) {
            fprintf(result, "ja ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JAE) {
            fprintf(result, "jae ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JB) {
            fprintf(result, "jb ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JBE) {
            fprintf(result, "jbe ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JE) {
            fprintf(result, "je ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JNE) {
            fprintf(result, "jne ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_JMP) {
            fprintf(result, "jmp ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_CALL) {
            fprintf(result, "call ");
            i++;
            fprintf(result, "%d\n", disasmblr->buf_input[i]);
        }
        else if(disasmblr->buf_input[i] == CMD_RET) {
            fprintf(result, "ret\n");
        }
        else if(disasmblr->buf_input[i] == CMD_DRAW) {
            fprintf(result, "draw\n");
        }
        else if(disasmblr->buf_input[i] == CMD_COUT) {
            fprintf(result, "cout\n");
        }
        else {
            fprintf(stderr, "SNTXERR: '%d'\n", disasmblr->buf_input[i]);
        }
    }

    MY_ASSERT(fclose(result) == 0, FILE_ERROR);

}

void PrintArgument(int* buff, size_t* i, FILE* result, int* code_error) {

    MY_ASSERT(result != NULL, FILE_ERROR);
    MY_ASSERT(buff != NULL, PTR_ERROR);

    if((buff[*i] & REG_MASK) && (buff[*i] & ARGC_MASK)) {
        (*i)++;
        PrintRegs(buff[*i], result, code_error);
        fprintf(result, " + ");
        (*i)++;
        fprintf(result, "%d", buff[*i]);
    }
    else if(buff[*i] & REG_MASK) {
        (*i)++;
        PrintRegs(buff[*i], result, code_error);
    }
    else if(buff[*i] & ARGC_MASK) {
        (*i)++;
        fprintf(result, "%d", buff[*i]);
    }

}

void PrintRegs(int reg, FILE* result, int* code_error) {

    MY_ASSERT(result != NULL, FILE_ERROR);

    switch(reg) {
        case AX:
            fprintf(result, "ax");
            break;
        case BX:
            fprintf(result, "bx");
            break;
        case CX:
            fprintf(result, "cx");
            break;
        case DX:
            fprintf(result, "dx");
            break;
        default:
            fprintf(stderr, "SNTXERR(reg): '%d'\n", reg);
            break;
    }

}

void DisasmDtor(Disassembler* disasmblr, int* code_error) {

    MY_ASSERT(disasmblr != NULL, PTR_ERROR);

    disasmblr->file_name_input = NULL;
    disasmblr->file_name_output = NULL;

    free(disasmblr->buf_input);
    disasmblr->buf_input = NULL;

    disasmblr->size_file = 0;

}