#include "asm.hpp"

long int count_size_file(FILE* program) {
    MY_ASSERT(program != NULL, FILE_ERROR);
    struct stat buff = {};
    fstat(fileno(program), &buff);
    return buff.st_size;
}

Errors AsmCtor(Assembler* asmblr) {

    Errors err = NO_ERROR;

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->file_name_input = "program.txt";
    asmblr->file_name_print_txt = "result.txt";

    CHECKED_ ProgramInput(asmblr);

    asmblr->n_cmd = count_num_of_cmds(asmblr);
    asmblr->n_words = count_num_of_words(asmblr);

    return NO_ERROR;
}

Errors ProgramInput(Assembler* asmblr) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    FILE* program = fopen(asmblr->file_name_input, "rb");
    MY_ASSERT(program != NULL, FILE_ERROR);

    asmblr->size_file = count_size_file(program);

    asmblr->buf_input = (char*)calloc(asmblr->size_file, sizeof(char));
    MY_ASSERT(asmblr->buf_input, PTR_ERROR);

    fread(asmblr->buf_input, sizeof(char), asmblr->size_file, program);

    MY_ASSERT(fclose(program) == 0,FILE_ERROR);

    return NO_ERROR;
}

int count_num_of_words(const Assembler* asmblr) {
    int counter_words = 0;

    for(size_t i = 0; i < asmblr->size_file; i++) {
        if(isspace(asmblr->buf_input[i])) {
            if(isspace(asmblr->buf_input[i + 1])) {
                continue;
            }
            counter_words++;
        }
    }

    return counter_words + 1;
}

int count_num_of_cmds(const Assembler* asmblr) {
    int counter_cmds = 0;

    for(size_t i = 0; i < asmblr->size_file; i++) {
        if(asmblr->buf_input[i] == '\n') {
            if(asmblr->buf_input[i + 1] == '\n') {
                continue;
            }
            counter_cmds++;
        }
    }

    return counter_cmds + 1;
}

Errors Parcing(Assembler* asmblr) {

    asmblr->cmd = (Command*) calloc(asmblr->n_cmd, sizeof(Command));
    MY_ASSERT(asmblr->cmd, PTR_ERROR);

    asmblr->cmd[0].cmd = asmblr->buf_input;
    int j = 1;

    for(size_t i = 0; i < asmblr->size_file; i++) {

        if(asmblr->buf_input[i] == '\n') {

            while(isspace(asmblr->buf_input[i + 1])) {
                asmblr->buf_input[i] = '\0';
                i++;
            }

            asmblr->buf_input[i] = '\0';
            asmblr->cmd[j].cmd = (asmblr->buf_input + i + 1);
            j++;
        }
    }

    return NO_ERROR;
}

Errors CommandsParcing(Assembler* asmblr) {
    MY_ASSERT(asmblr, PTR_ERROR);

    asmblr->buf_output = (int*)calloc(asmblr->n_words, sizeof(int));
    MY_ASSERT(asmblr->buf_output, PTR_ERROR);

    for(size_t i = 0, j = 0; i < asmblr->n_cmd; i++, j++) {
        char* cmd = asmblr->cmd[i].cmd;

        if (!strcmp(cmd, "hlt")) {
            asmblr->cmd[i].cmd_code = CMD_HLT;
            asmblr->buf_output[j] = CMD_HLT;
        }
        else if (!strcmp(cmd, "dump")) {
            asmblr->cmd[i].cmd_code = CMD_DUMP;
            asmblr->buf_output[j] = CMD_DUMP;
        }
        else if (!strcmp(cmd, "out")) {
            asmblr->cmd[i].cmd_code = CMD_OUT;
            asmblr->buf_output[j] = CMD_OUT;
        }
        else if (!strcmp(cmd, "in")) {
            asmblr->cmd[i].cmd_code = CMD_IN;
            asmblr->buf_output[j] = CMD_IN;
        }
        else if (!strncmp(cmd, "push", 4)) {
            char* argc = &(asmblr->cmd[i].cmd[5]);
            int cmd_check = CMD_PUSH;

            asmblr->cmd[i].cmd_code = CMD_PUSH;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmd[i].cmd_code;
            j++;
            if(asmblr->cmd[i].cmd_code == (cmd_check | argc_mask)) {
                asmblr->buf_output[j] = asmblr->cmd[i].argc;
            }
            else if(asmblr->cmd[i].cmd_code == (cmd_check | reg_mask)) {
                asmblr->buf_output[j] = asmblr->cmd[i].reg;
            }
        }
        else if (!strncmp(cmd, "pop", 3)) {
            char* argc = &(asmblr->cmd[i].cmd[4]);
            int cmd_check = CMD_POP;

            asmblr->cmd[i].cmd_code = CMD_POP;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmd[i].cmd_code;
            if(asmblr->cmd[i].cmd_code == (cmd_check | reg_mask)) {
                j++;
                asmblr->buf_output[j] = asmblr->cmd[i].reg;
            }
        }
        else if (!strcmp(cmd, "add")) {
            asmblr->cmd[i].cmd_code = CMD_ADD;
            asmblr->buf_output[j] = CMD_ADD;
        }
        else if (!strcmp(cmd, "sub")) {
            asmblr->cmd[i].cmd_code = CMD_SUB;
            asmblr->buf_output[j] = CMD_SUB;
        }
        else if (!strcmp(cmd, "mul")) {
            asmblr->cmd[i].cmd_code = CMD_MUL;
            asmblr->buf_output[j] = CMD_MUL;
        }
        else if (!strcmp(cmd, "div")) {
            asmblr->cmd[i].cmd_code = CMD_DIV;
            asmblr->buf_output[j] = CMD_DIV;
        }
        else if (!strcmp(cmd, "sqrt")) {
            asmblr->cmd[i].cmd_code = CMD_SQRT;
            asmblr->buf_output[j] = CMD_SQRT;
        }
        else if (!strcmp(cmd, "sin")) {
            asmblr->cmd[i].cmd_code = CMD_SIN;
            asmblr->buf_output[j] = CMD_SIN;
        }
        else if (!strcmp(cmd, "cos")) {
            asmblr->cmd[i].cmd_code = CMD_COS;
            asmblr->buf_output[j] = CMD_COS;
        }
        else {
            fprintf(stderr, "SNTXERR: '%s'\n", cmd);
        }
    }

    return NO_ERROR;
}

void ArgumentsParcing(Assembler* asmblr, size_t i, char* argc) {
    int arg = atoi(argc);

    if(*(argc + 1) == 'x') {
        asmblr->cmd[i].cmd_code |= reg_mask;

        if(*argc == 'a') {
            asmblr->cmd[i].reg = ax;
        }
        else if(*argc == 'b') {
            asmblr->cmd[i].reg = bx;
        }
        else if(*argc == 'c') {
            asmblr->cmd[i].reg = cx;
        }
        else if(*argc == 'd') {
            asmblr->cmd[i].reg = dx;
        }
    }
    else if(arg){
        asmblr->cmd[i].argc = arg;
        asmblr->cmd[i].cmd_code |= argc_mask;
    }
    else {
        if(!strcmp((argc - 4), "pop")) {
            return;
        }
        fprintf(stderr, "%d SNTXERR: '%s'\n", __LINE__, asmblr->cmd[i].cmd);
    }
}

Errors Output(Assembler* asmblr) {
    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    FILE* result = fopen(asmblr->file_name_print_txt, "w + b");
    MY_ASSERT(result != NULL, FILE_ERROR);

    printf("%ld\n", fwrite(asmblr->buf_output, sizeof(int), asmblr->n_words, result));

    MY_ASSERT(fclose(result) == 0, FILE_ERROR);

    return NO_ERROR;
}

