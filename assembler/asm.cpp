#include "asm.hpp"

long int count_size_file(FILE* program) {
    MY_ASSERT(program != NULL, FILE_ERROR);
    struct stat buff = {};
    fstat(fileno(program), &buff);
    return buff.st_size;
}

Errors AsmCtor(Assembler* asmblr) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->file_name_input = "program.txt";
    asmblr->file_name_print_txt = "result.txt";

    CHECKED_ ProgramInput(asmblr);

    asmblr->n_cmd = count_num_of_cmds(asmblr);
    asmblr->n_words = count_num_of_words(asmblr);

    asmblr->lbls = (Label*)calloc(NUM_OF_LABELS, sizeof(Label));
    MY_ASSERT(asmblr->lbls != NULL, PTR_ERROR);

    for(int i = 0; i < NUM_OF_LABELS; i++) {
        asmblr->lbls[i].address = -1;
    }

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

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->cmds = (Command*) calloc(asmblr->n_cmd, sizeof(Command));
    MY_ASSERT(asmblr->cmds, PTR_ERROR);

    asmblr->cmds[0].cmd = asmblr->buf_input;
    int j = 1;

    for(size_t i = 0; i < asmblr->size_file; i++) {

        if(asmblr->buf_input[i] == '\n') {

            while(isspace(asmblr->buf_input[i + 1])) {
                asmblr->buf_input[i] = '\0';
                i++;
            }

            asmblr->buf_input[i] = '\0';
            asmblr->cmds[j].cmd = (asmblr->buf_input + i + 1);
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
        char* cmd = asmblr->cmds[i].cmd;

        if (!strcmp(cmd, "hlt")) {
            asmblr->cmds[i].cmd_code = CMD_HLT;
            asmblr->buf_output[j] = CMD_HLT;
        }
        else if (!strcmp(cmd, "dump")) {
            asmblr->cmds[i].cmd_code = CMD_DUMP;
            asmblr->buf_output[j] = CMD_DUMP;
        }
        else if (!strcmp(cmd, "out")) {
            asmblr->cmds[i].cmd_code = CMD_OUT;
            asmblr->buf_output[j] = CMD_OUT;
        }
        else if (!strcmp(cmd, "in")) {
            asmblr->cmds[i].cmd_code = CMD_IN;
            asmblr->buf_output[j] = CMD_IN;
        }
        else if (!strncmp(cmd, "push", 4)) {
            char* argc = &(asmblr->cmds[i].cmd[5]);
            int cmd_check = CMD_PUSH;

            asmblr->cmds[i].cmd_code = CMD_PUSH;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            if(asmblr->cmds[i].cmd_code == (cmd_check | argc_mask)) {
                asmblr->buf_output[j] = asmblr->cmds[i].argc;
            }
            else if(asmblr->cmds[i].cmd_code == (cmd_check | reg_mask)) {
                asmblr->buf_output[j] = asmblr->cmds[i].reg;
            }
        }
        else if (!strncmp(cmd, "pop", 3)) {
            char* argc = &(asmblr->cmds[i].cmd[4]);
            int cmd_check = CMD_POP;

            asmblr->cmds[i].cmd_code = CMD_POP;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            if(asmblr->cmds[i].cmd_code == (cmd_check | reg_mask)) {
                j++;
                asmblr->buf_output[j] = asmblr->cmds[i].reg;
            }
        }
        else if (!strcmp(cmd, "add")) {
            asmblr->cmds[i].cmd_code = CMD_ADD;
            asmblr->buf_output[j] = CMD_ADD;
        }
        else if (!strcmp(cmd, "sub")) {
            asmblr->cmds[i].cmd_code = CMD_SUB;
            asmblr->buf_output[j] = CMD_SUB;
        }
        else if (!strcmp(cmd, "mul")) {
            asmblr->cmds[i].cmd_code = CMD_MUL;
            asmblr->buf_output[j] = CMD_MUL;
        }
        else if (!strcmp(cmd, "div")) {
            asmblr->cmds[i].cmd_code = CMD_DIV;
            asmblr->buf_output[j] = CMD_DIV;
        }
        else if (!strcmp(cmd, "sqrt")) {
            asmblr->cmds[i].cmd_code = CMD_SQRT;
            asmblr->buf_output[j] = CMD_SQRT;
        }
        else if (!strcmp(cmd, "sin")) {
            asmblr->cmds[i].cmd_code = CMD_SIN;
            asmblr->buf_output[j] = CMD_SIN;
        }
        else if (!strcmp(cmd, "cos")) {
            asmblr->cmds[i].cmd_code = CMD_COS;
            asmblr->buf_output[j] = CMD_COS;
        }
        else if (!strncmp(cmd, "jae", 3)) {
            char* argc = &(asmblr->cmds[i].cmd[4]);

            asmblr->cmds[i].cmd_code = CMD_JAE;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "ja", 2)) {
            char* argc = &(asmblr->cmds[i].cmd[3]);

            asmblr->cmds[i].cmd_code = CMD_JA;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "jbe", 3)) {
            char* argc = &(asmblr->cmds[i].cmd[4]);

            asmblr->cmds[i].cmd_code = CMD_JBE;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "jb", 2)) {
            char* argc = &(asmblr->cmds[i].cmd[3]);

            asmblr->cmds[i].cmd_code = CMD_JB;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "je", 2)) {
            char* argc = &(asmblr->cmds[i].cmd[3]);

            asmblr->cmds[i].cmd_code = CMD_JE;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "jne", 3)) {
            char* argc = &(asmblr->cmds[i].cmd[4]);

            asmblr->cmds[i].cmd_code = CMD_JNE;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }
        else if (!strncmp(cmd, "jmp", 3)) {
            char* argc = &(asmblr->cmds[i].cmd[4]);

            asmblr->cmds[i].cmd_code = CMD_JMP;
            ArgumentsParcing(asmblr, i, argc);
            asmblr->buf_output[j] = asmblr->cmds[i].cmd_code;
            j++;
            asmblr->buf_output[j] = asmblr->cmds[i].label;
        }

        else if(strchr(cmd, ':') != NULL) {
            label_insert(cmd, asmblr, j);
        }

        else {
            fprintf(stderr, "SNTXERR: '%s'\n", cmd);
        }
    }

    return NO_ERROR;
}

void ArgumentsParcing(Assembler* asmblr, size_t i, char* argc) {
    int param = atoi(argc);

    if(*(argc + 1) == 'x') {
        asmblr->cmds[i].cmd_code |= reg_mask;

        if(*argc == 'a') {
            asmblr->cmds[i].reg = ax;
        }
        else if(*argc == 'b') {
            asmblr->cmds[i].reg = bx;
        }
        else if(*argc == 'c') {
            asmblr->cmds[i].reg = cx;
        }
        else if(*argc == 'd') {
            asmblr->cmds[i].reg = dx;
        }
    }
    else if(strchr(argc, ':') != NULL) {
        int addr = label_find(argc, asmblr);
        asmblr->cmds[i].label = addr;
    }
    else if(*(argc - 3) == 'j' || *(argc - 4) == 'j') {
        asmblr->cmds[i].label = param;
    }
    else if(!strcmp((argc - 4), "pop")) {
        return;
    }
    else {
        asmblr->cmds[i].argc = param;
        asmblr->cmds[i].cmd_code |= argc_mask;
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

void label_insert(char* cmd, Assembler* asmblr, size_t j) {
    size_t i = 0;
    while(asmblr->lbls[i].address != -1) {
        if(i > NUM_OF_LABELS) {
            return;
        }
        i++;
    }
    asmblr->lbls[i].address = j;
    asmblr->lbls[i].name = cmd;
}

int label_find(char* cmd, Assembler* asmblr) {
    size_t i = 0;
    while(strcmp(cmd, asmblr->lbls[i].name)) {
        MY_ASSERT(i <= NUM_OF_LABELS, SIZE_ERROR);
        i++;
    }
    int addr = asmblr->lbls[i].address;
    return addr;
}

void AsmDump(Assembler* asmblr) {
    fprintf(stderr, "\n");
    for(size_t i = 0; i < NUM_OF_LABELS; i++) {
        fprintf(stderr, "%d %s", asmblr->lbls[i].address, asmblr->lbls[i].name);
    }
}

