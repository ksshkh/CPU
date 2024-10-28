#include "asm.hpp"

void AsmCtor(Assembler* asmblr, int argc, char* argv[], int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);
    MY_ASSERT(argv != NULL, PTR_ERROR);

    if(argc == 1) {
        asmblr->file_name_input = "../programs/program.txt";
    }
    else if(argc == 2) {
        asmblr->file_name_input = argv[1];
    }

    asmblr->file_name_output = "../programs/result.bin";

    PROGRAM_INPUT(asmblr);

    ELEMENTS_COUNTER(asmblr);

    asmblr->lbls = (Label*)calloc(NUM_OF_LABELS, sizeof(Label));
    MY_ASSERT(asmblr->lbls != NULL, PTR_ERROR);

    for(int i = 0; i < NUM_OF_LABELS; i++) {
        asmblr->lbls[i].address = VALUE_DEFAULT;
    }

    asmblr->buf_output = (int*)calloc(asmblr->n_words, sizeof(int));
    MY_ASSERT(asmblr->buf_output != NULL, PTR_ERROR);

}

void ProgramInput(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    FILE* program = fopen(asmblr->file_name_input, "rb");
    MY_ASSERT(program != NULL, FILE_ERROR);

    asmblr->size_file = COUNT_SIZE_FILE(program);

    asmblr->buf_input = (char*)calloc(asmblr->size_file, sizeof(char));
    MY_ASSERT(asmblr->buf_input, PTR_ERROR);

    fread(asmblr->buf_input, sizeof(char), asmblr->size_file, program);

    MY_ASSERT(fclose(program) == 0, FILE_ERROR);

}

void ElemetsCounter(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    int counter_words  = 0;
    int counter_labels = 0;
    int counter_cmds   = 0;

    bool is_begin = false;

    for(size_t i = 0; i < asmblr->size_file; i++) {

        if(isspace(asmblr->buf_input[i]) && is_begin) {

            if(asmblr->buf_input[i] == '\n') {
                if(asmblr->buf_input[i - 1] != '\n') {
                    counter_cmds++;
                }
            }

            if(isspace(asmblr->buf_input[i + 1])) {
                continue;
            }

            counter_words++;
        }
        else if(!isspace(asmblr->buf_input[i])) {
            is_begin = true;
        }

        if(asmblr->buf_input[i] == '+') {
                counter_words--;
        }
        else if(asmblr->buf_input[i] == ':') {
                counter_labels++;
        }
    }

    counter_words = counter_words - counter_labels + 1;

    asmblr->n_words = counter_words;
    asmblr->n_cmd = counter_cmds + 1;

}

void BufferParcing(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->cmds = (Command*)calloc(asmblr->n_cmd, sizeof(Command));
    MY_ASSERT(asmblr->cmds, PTR_ERROR);

    size_t i = 0;
    while(isspace(asmblr->buf_input[i])) {
        i++;
    }

    asmblr->cmds[0].cmd = asmblr->buf_input + i;
    asmblr->cmds[0].label = VALUE_DEFAULT;
    asmblr->cmds[0].cmd_code = CMD_DEFAULT;
    int j = 1;

    for(i; i < asmblr->size_file; i++) {

        if(asmblr->buf_input[i] == '\n') {

            while(isspace(asmblr->buf_input[i + 1])) {
                asmblr->buf_input[i] = '\0';
                i++;
            }

            asmblr->buf_input[i] = '\0';
            asmblr->cmds[j].cmd = (asmblr->buf_input + i + 1);
            asmblr->cmds[j].label = VALUE_DEFAULT;
            asmblr->cmds[j].cmd_code = CMD_DEFAULT;
            j++;
        }
    }

}

void CommandsParcing(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->buf_output = (int*)calloc(asmblr->n_words, sizeof(int));
    MY_ASSERT(asmblr->buf_output != NULL, PTR_ERROR);

    for(int twice_comp = 0; twice_comp < 2; twice_comp++) {

        int buff_indx = 0;

        for(size_t i = 0; i < asmblr->n_cmd; i++, buff_indx++) {

            char* cmd = asmblr->cmds[i].cmd;

            if(!strcmp(cmd, "hlt")) {
                asmblr->cmds[i].cmd_code = CMD_HLT;
            }
            else if(!strcmp(cmd, "dump")) {
                asmblr->cmds[i].cmd_code = CMD_DUMP;
            }
            else if(!strcmp(cmd, "out")) {
                asmblr->cmds[i].cmd_code = CMD_OUT;
            }
            else if(!strcmp(cmd, "in")) {
                asmblr->cmds[i].cmd_code = CMD_IN;
            }
            else if(!strncmp(cmd, "push", 4)) {
                char* argc = &(asmblr->cmds[i].cmd[5]);

                asmblr->cmds[i].cmd_code = CMD_PUSH;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;

                if((asmblr->cmds[i].cmd_code & ARGC_MASK) && (asmblr->cmds[i].cmd_code & REG_MASK)) {
                    buff_indx++;
                }
            }
            else if(!strncmp(cmd, "pop", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_POP;
                ARGUMENTS_PARCING(asmblr, i, argc);

                if((asmblr->cmds[i].cmd_code & ARGC_MASK) || (asmblr->cmds[i].cmd_code & REG_MASK)) {
                    buff_indx++;
                }
            }
            else if(!strcmp(cmd, "add")) {
                asmblr->cmds[i].cmd_code = CMD_ADD;
            }
            else if(!strcmp(cmd, "sub")) {
                asmblr->cmds[i].cmd_code = CMD_SUB;
            }
            else if(!strcmp(cmd, "mul")) {
                asmblr->cmds[i].cmd_code = CMD_MUL;
            }
            else if(!strcmp(cmd, "div")) {
                asmblr->cmds[i].cmd_code = CMD_DIV;
            }
            else if(!strcmp(cmd, "sqrt")) {
                asmblr->cmds[i].cmd_code = CMD_SQRT;
            }
            else if(!strcmp(cmd, "sin")) {
                asmblr->cmds[i].cmd_code = CMD_SIN;
            }
            else if(!strcmp(cmd, "cos")) {
                asmblr->cmds[i].cmd_code = CMD_COS;
            }
            else if(!strncmp(cmd, "jae", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JAE;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "ja", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JA;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "jbe", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JBE;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "jb", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JB;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "je", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JE;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "jne", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JNE;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "jmp", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JMP;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strncmp(cmd, "call", 4)) {
                char* argc = &(asmblr->cmds[i].cmd[5]);
                asmblr->cmds[i].cmd_code = CMD_CALL;
                ARGUMENTS_PARCING(asmblr, i, argc);
                buff_indx++;
            }
            else if(!strcmp(cmd, "ret")) {
                asmblr->cmds[i].cmd_code = CMD_RET;
            }
            else if(!strcmp(cmd, "cout")) {
                asmblr->cmds[i].cmd_code = CMD_COUT;
            }
            else if(!strcmp(cmd, "draw")) {
                asmblr->cmds[i].cmd_code = CMD_DRAW;
            }
            else if(strchr(cmd, ':') != NULL) {
                char* doubledot = strchr(cmd, ':');
                *doubledot = '\0';
                LABEL_INSERT(cmd, asmblr, &buff_indx);
                *doubledot = ':';
            }
            else {
                fprintf(stderr, "SNTXERR: '%s'\n", cmd);
            }
        }
    }

}

void ArgumentsParcing(Assembler* asmblr, size_t i, char* argc, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);
    MY_ASSERT(argc != NULL, PTR_ERROR);

    char* close_sym = NULL;
    char* space_sym = NULL;

    if(strlen(argc - 4) == 3) { //just pop
        return;
    }

    if(*argc == '[') {
        asmblr->cmds[i].cmd_code |= MEM_MASK;
        close_sym = strchr(argc, ']');

        if(close_sym != NULL) {
            *close_sym = '\0';
        }
        else {
            fprintf(stderr, "SNTXERR: '%s'\n", argc);
        }

        argc += 1;
    }

    if(strchr(argc, '+') != NULL) {
        space_sym = strchr(argc, ' ');
        *space_sym = '\0';

        char* reg = argc;
        char* arg = argc + 5;

        ARGUMENTS_HANDLING(asmblr, i, reg);
        ARGUMENTS_HANDLING(asmblr, i, arg);

        *space_sym = ' ';
    }
    else if(*(argc - 3) == 'j' || *(argc - 4) == 'j' || *(argc - 2) == 'l') {
        if(!isdigit(*argc)) {
            size_t label_len = strlen(argc);
            int addr = LABEL_FIND(argc, asmblr, label_len);

            if(addr != VALUE_DEFAULT) {
                asmblr->cmds[i].label = addr;
            }
            else {
                LABEL_INSERT(argc, asmblr, &addr);
            }
        }
        else {
            ARGUMENTS_HANDLING(asmblr, i, argc);
        }

    }
    else {
        ARGUMENTS_HANDLING(asmblr, i, argc);
    }

    if(close_sym != NULL) {
        *close_sym = ']';
    }

}

void ArgumentsHandling(Assembler* asmblr, size_t i, char* argc, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);
    MY_ASSERT(argc != NULL, PTR_ERROR);

    int param = atoi(argc);
    if(*(argc + 1) == 'x') {
        asmblr->cmds[i].cmd_code |= REG_MASK;

        if(*argc == 'a') {
            asmblr->cmds[i].reg = AX;
        }
        else if(*argc == 'b') {
            asmblr->cmds[i].reg = BX;
        }
        else if(*argc == 'c') {
            asmblr->cmds[i].reg = CX;
        }
        else if(*argc == 'd') {
            asmblr->cmds[i].reg = DX;
        }
    }
    else {
        asmblr->cmds[i].argc = param;
        asmblr->cmds[i].cmd_code |= ARGC_MASK;
    }

    if(*(argc - 3) == 'j' || *(argc - 4) == 'j') {
        asmblr->cmds[i].label = param;
        asmblr->cmds[i].cmd_code ^= ARGC_MASK;
    }

}

void BufferFilling(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    int buff_indx = 0;
    for(size_t i = 0; i < asmblr->n_cmd; i++) {

        if(asmblr->cmds[i].cmd_code == CMD_DEFAULT) {
            continue;
        }

        asmblr->buf_output[buff_indx++] = asmblr->cmds[i].cmd_code;

        if(asmblr->cmds[i].label != VALUE_DEFAULT) {
            asmblr->buf_output[buff_indx++] = asmblr->cmds[i].label;
        }

        if((asmblr->cmds[i].cmd_code & REG_MASK) && asmblr->cmds[i].cmd_code != CMD_HLT) {
            asmblr->buf_output[buff_indx++] = asmblr->cmds[i].reg;
        }

        if((asmblr->cmds[i].cmd_code & ARGC_MASK) && asmblr->cmds[i].cmd_code != CMD_HLT) {
            asmblr->buf_output[buff_indx++] = asmblr->cmds[i].argc;
        }
    }

}

void LabelInsert(char* cmd, Assembler* asmblr, int* buff_indx, int* code_error) {

    MY_ASSERT(asmblr    != NULL, PTR_ERROR);
    MY_ASSERT(cmd       != NULL, PTR_ERROR);
    MY_ASSERT(buff_indx != NULL, PTR_ERROR);

    size_t i = 0;
    while(i < NUM_OF_LABELS) {

        if(asmblr->lbls[i].name != NULL) {
            if(!strcmp(cmd, asmblr->lbls[i].name)) {
                break;
            }
        }
        else if(asmblr->lbls[i].address == VALUE_DEFAULT) {
            break;
        }

        i++;
    }

    asmblr->lbls[i].address = *buff_indx;
    asmblr->lbls[i].name = cmd;
    *(buff_indx) -= 1;

}

void Output(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    FILE* result = fopen(asmblr->file_name_output, "w + b");
    MY_ASSERT(result != NULL, FILE_ERROR);

    fwrite(asmblr->buf_output, sizeof(int), asmblr->n_words, result);

    MY_ASSERT(fclose(result) == 0, FILE_ERROR);

}

int LabelFind(char* cmd, Assembler* asmblr, size_t label_len, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);
    MY_ASSERT(cmd != NULL, PTR_ERROR);

    size_t i = 0;

    while(i < NUM_OF_LABELS) {

        if(asmblr->lbls[i].name != NULL) {
            if(!strncmp(cmd, asmblr->lbls[i].name, label_len)) {
                break;
            }
        }
        i++;
    }

    if(i >= NUM_OF_LABELS) {
        return VALUE_DEFAULT;
    }

    int addr = asmblr->lbls[i].address;

    return addr;
}

void AsmDump(Assembler* asmblr, int* code_error) {

    FILE* debug = fopen("../debug/assembler_dump.txt", "a");

    if(debug != NULL) {

        MY_STRERR(debug);

        if(*code_error) {
            fprintf(stderr, "code error %d\n", *code_error);
        }

        if(asmblr != NULL) {
            fprintf(debug, "number of commands: %ld\nmumber of words: %ld\n", asmblr->n_cmd, asmblr->n_words);

            if(asmblr->cmds != NULL) {
                fprintf(debug, "\ncommands table:\n\n");

                for(size_t i = 0; i < asmblr->n_cmd; i++) {
                    fprintf(debug, "%s\n", asmblr->cmds[i].cmd);
                    fprintf(debug, "cmd code: %d argument: %d register: %d label: %d\n", asmblr->cmds[i].cmd_code, asmblr->cmds[i].argc, asmblr->cmds[i].reg, asmblr->cmds[i].label);
                }
            }
            else {
                fprintf(debug, "no commands table\n");
            }

            if(asmblr->lbls != NULL) {
                fprintf(debug, "\nlabels table:\n\n");

                for(size_t i = 0; i < NUM_OF_LABELS; i++) {
                    fprintf(debug, "address: %d name: %s\n", asmblr->lbls[i].address, asmblr->lbls[i].name);
                }
            }
            else {
                fprintf(debug, "no labels table\n");
            }

            if(asmblr->buf_output != NULL) {
                fprintf(debug, "\noutput buffer:\n\n");

                for(size_t i = 0; i < asmblr->n_words; i++) {
                    fprintf(debug, "%d ", asmblr->buf_output[i]);
                }

                fprintf(debug, "\n");
            }
            else {
                fprintf(debug, "no output buffer\n");
            }
        }
        else {
            fprintf(debug, "no asmblr\n");
        }

        if(fclose(debug)) {
            fprintf(stderr, "file did not close\n");
        }
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

void AsmDtor(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->file_name_input = NULL;
    asmblr->file_name_output = NULL;

    free(asmblr->buf_input);
    asmblr->buf_input = NULL;

    free(asmblr->buf_output);
    asmblr->buf_output = NULL;

    asmblr->size_file = 0;
    asmblr->n_cmd = 0;
    asmblr->n_words = 0;

    free(asmblr->cmds);
    asmblr->cmds = NULL;

    free(asmblr->lbls);
    asmblr->lbls = NULL;

}

