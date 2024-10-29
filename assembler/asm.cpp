#include "asm.hpp"

static const char* DEBUG_FILE_NAME = "../debug/assembler_dump.txt";

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

    ProgramInput(asmblr, code_error);

    ElementsCounter(asmblr, code_error);

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

    asmblr->size_file = count_size_file(program, code_error);

    asmblr->buf_input = (char*)calloc(asmblr->size_file, sizeof(char));
    MY_ASSERT(asmblr->buf_input, PTR_ERROR);

    fread(asmblr->buf_input, sizeof(char), asmblr->size_file, program);

    MY_ASSERT(fclose(program) == 0, FILE_ERROR);

}

void ElementsCounter(Assembler* asmblr, int* code_error) {

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

    counter_words   = counter_words - counter_labels + 1;

    asmblr->n_words = counter_words;
    asmblr->n_cmd   = counter_cmds + 1;

}

void BufferParcing(Assembler* asmblr, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);

    asmblr->cmds = (Command*)calloc(asmblr->n_cmd, sizeof(Command));
    MY_ASSERT(asmblr->cmds, PTR_ERROR);

    size_t i = 0;
    while(isspace(asmblr->buf_input[i])) {
        i++;
    }

    asmblr->cmds[0].cmd      = asmblr->buf_input + i;
    asmblr->cmds[0].label    = VALUE_DEFAULT;
    asmblr->cmds[0].cmd_code = CMD_DEFAULT;
    int j = 1;

    for(i; i < asmblr->size_file; i++) {

        if(asmblr->buf_input[i] == '\n') {

            while(isspace(asmblr->buf_input[i + 1])) {
                asmblr->buf_input[i] = '\0';
                i++;
            }

            asmblr->buf_input[i] = '\0';
            asmblr->cmds[j].cmd      = (asmblr->buf_input + i + 1);
            asmblr->cmds[j].label    = VALUE_DEFAULT;
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

        int ip = 0;

        for(size_t i = 0; i < asmblr->n_cmd; i++) {

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
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "pop", 3)) {
                asmblr->cmds[i].cmd_code = CMD_POP;

                if(strlen(cmd) != 3) {
                    char* argc = &(asmblr->cmds[i].cmd[4]);
                    ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
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
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "ja", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JA;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "jbe", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JBE;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "jb", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JB;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "je", 2)) {
                char* argc = &(asmblr->cmds[i].cmd[3]);

                asmblr->cmds[i].cmd_code = CMD_JE;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "jne", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JNE;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "jmp", 3)) {
                char* argc = &(asmblr->cmds[i].cmd[4]);

                asmblr->cmds[i].cmd_code = CMD_JMP;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
            }
            else if(!strncmp(cmd, "call", 4)) {
                char* argc = &(asmblr->cmds[i].cmd[5]);
                asmblr->cmds[i].cmd_code = CMD_CALL;
                ArgumentsParcing(&asmblr->cmds[i], asmblr, argc, code_error);
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
                LabelInsert(cmd, asmblr, &ip, code_error);
                *doubledot = ':';
            }
            else {
                fprintf(stderr, "SNTXERR: '%s'\n", cmd);
                continue;
            }

            BufferFilling(asmblr->cmds[i], asmblr->buf_output, &ip, code_error);
        }
    }
}

void ArgumentsParcing(Command* command_struct, Assembler* asmblr, char* argc, int* code_error) {

    MY_ASSERT(argc != NULL, PTR_ERROR);
    MY_ASSERT(command_struct != NULL, PTR_ERROR);

    char* close_sym = NULL;

    if(*argc == '[') {
        command_struct->cmd_code |= MEM_MASK;
        close_sym = strchr(argc, ']');

        if(close_sym != NULL) {
            *close_sym = '\0';
        }
        else {
            fprintf(stderr, "SNTXERR: '%s'\n", argc);
        }

        argc += 1;
    }

    GetArg(command_struct, asmblr, argc, code_error);

    if(close_sym != NULL) {
        *close_sym = ']';
    }
}

void GetArg(Command* command_struct, Assembler* asmblr, char* argc, int* code_error) {

    MY_ASSERT(argc != NULL, PTR_ERROR);
    MY_ASSERT(command_struct != NULL, PTR_ERROR);

    if(RegParcing(command_struct, argc, code_error) != REG_DEFAULT) {
        command_struct->reg = RegParcing(command_struct, argc, code_error);
        char* arg = argc + 5;
        command_struct->argc = ValueParcing(command_struct, arg, code_error);
    }
    else if(ValueParcing(command_struct, argc, code_error) != NO_ARGUMENT) {
        command_struct->argc = ValueParcing(command_struct, argc, code_error);
    }
    else {
        size_t label_len = strlen(argc);
        int addr = LabelFind(argc, asmblr, label_len, code_error);

        if(addr != VALUE_DEFAULT) {
            command_struct->label = addr;
        }
        else {
            LabelInsert(argc, asmblr, &addr, code_error);
        }
    }
}

StackElem_t ValueParcing(Command* command_struct, char* argc, int* code_error) {

    MY_ASSERT(argc != NULL, PTR_ERROR);

    if(isdigit(*argc) != 0) {
        command_struct->cmd_code |= ARGC_MASK;
        StackElem_t param = (StackElem_t)atoi(argc);
        return param;
    }

    return NO_ARGUMENT;
}

Registers RegParcing(Command* command_struct, char* argc, int* code_error) {

    MY_ASSERT(argc != NULL, PTR_ERROR);

    if(*(argc + 1) == 'x') {
        command_struct->cmd_code |= REG_MASK;

        if(*argc == 'a') {
            return AX;
        }
        else if(*argc == 'b') {
            return BX;
        }
        else if(*argc == 'c') {
            return CX;
        }
        else if(*argc == 'd') {
            return DX;
        }
        else {
            fprintf(stderr, "SNTXERR: '%s'\n", argc);
        }
    }

    return REG_DEFAULT;
}

void BufferFilling(const Command command_struct, int* buf_output, int* ip, int* code_error) {

    MY_ASSERT(buf_output != NULL, PTR_ERROR);
    MY_ASSERT(ip         != NULL, PTR_ERROR);

    if(command_struct.cmd_code == CMD_DEFAULT) {
        (*ip)++;
        return;
    }

    buf_output[(*ip)++] = command_struct.cmd_code;

    if(command_struct.label != VALUE_DEFAULT) {
        buf_output[(*ip)++] = command_struct.label;
        return;
    }

    if((command_struct.cmd_code & REG_MASK) && command_struct.cmd_code != CMD_HLT) {
        buf_output[(*ip)++] = command_struct.reg;
    }

    if((command_struct.cmd_code & ARGC_MASK) && command_struct.cmd_code != CMD_HLT) {
        buf_output[(*ip)++] = command_struct.argc;
    }

    if(*(command_struct.cmd) == 'j' || !strncmp(command_struct.cmd, "call", 4)) {
        buf_output[(*ip)++] = command_struct.label;
    }

}

void LabelInsert(char* cmd, Assembler* asmblr, int* ip, int* code_error) {

    MY_ASSERT(asmblr != NULL, PTR_ERROR);
    MY_ASSERT(cmd    != NULL, PTR_ERROR);
    MY_ASSERT(ip     != NULL, PTR_ERROR);

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

    asmblr->lbls[i].address = *ip;
    asmblr->lbls[i].name    = cmd;
    *(ip) -= 1;

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
    MY_ASSERT(cmd    != NULL, PTR_ERROR);

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

    FILE* debug = fopen(DEBUG_FILE_NAME, "a");

    if(debug != NULL) {

        my_strerr(debug, code_error);

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

    asmblr->file_name_input  = NULL;
    asmblr->file_name_output = NULL;

    free(asmblr->buf_input);
    asmblr->buf_input  = NULL;

    free(asmblr->buf_output);
    asmblr->buf_output = NULL;

    asmblr->size_file = 0;
    asmblr->n_cmd     = 0;
    asmblr->n_words   = 0;

    free(asmblr->cmds);
    asmblr->cmds = NULL;

    free(asmblr->lbls);
    asmblr->lbls = NULL;

}

