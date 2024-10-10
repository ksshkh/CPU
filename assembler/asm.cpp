#include "asm.hpp"

long int count_size_file(FILE* program) {
    struct stat buff;
    fstat(fileno(program), &buff);
    return buff.st_size;
}

Errors BuffCtor(Buffer* buff) {
    MY_ASSERT(buff != NULL, PTR_ERROR);

    FILE* program = fopen("program.txt", "rb");

    if(program != NULL) {
        buff->size_of_file = count_size_file(program);

        buff->buffer = (char*)calloc(buff->size_of_file + 1, sizeof(char));
        fread(buff->buffer, sizeof(char), buff->size_of_file, program);
    }

    else {
        fprintf(stderr, "file with program did not open(\n");
    }

    if(fclose(program)) {
        fprintf(stderr, "file with program did not close(\n");
    }

    return NO_ERROR;
}

Errors PartBuffCtor(PartitionedBuffer* prtbuff, const Buffer* buff) {
    MY_ASSERT(buff != NULL, PTR_ERROR);
    MY_ASSERT(prtbuff != NULL, PTR_ERROR);

    prtbuff->size_of_part_buffer = count_num_of_symbols(buff);
    prtbuff->part_buffer = (char**)calloc(prtbuff->size_of_part_buffer, sizeof(char*));

    prtbuff->part_buffer[0] = buff->buffer;
    int j = 1;
    for(long int i = 0; i < buff->size_of_file; i++) {
        if(buff->buffer[i] == '\n' || buff->buffer[i] == ' ' || buff->buffer[i] == '\0') {
            prtbuff->part_buffer[j] = (buff->buffer + i + 1);
            buff->buffer[i] = '\0';
            j++;
        }
    }

    return NO_ERROR;
}

int count_num_of_symbols(const Buffer* buff) {
    int counter_symbols = 0;
    for (int i = 0; i <= buff->size_of_file; i++) {
        if (buff->buffer[i] == '\n' || buff->buffer[i] == ' ' || buff->buffer[i] == '\0') {
            counter_symbols++;
        }
    }
    return counter_symbols;
}

Errors AsmRun(const PartitionedBuffer* prtbuff) {
    MY_ASSERT(prtbuff != NULL, PTR_ERROR);
    FILE* code = fopen("code.txt", "w+");
    if(code != NULL) {
        for(int i = 0; i < prtbuff->size_of_part_buffer; i++) {
            char* cmd = prtbuff->part_buffer[i];
            if (!strcmp(cmd, "hlt")) {
                fprintf(code, "%d\n", CMD_HLT);
            }
            else if (!strcmp(cmd, "dump")) {
                fprintf(code, "%d\n", CMD_DUMP);
            }
            else if (!strcmp(cmd, "out")) {
                fprintf(code, "%d\n", CMD_OUT);
            }
            else if (!strcmp(cmd, "in")) {
                fprintf(code, "%d\n", CMD_IN);
            }
            else if (!strcmp(cmd, "push")) {
                i++;
                fprintf(code, "%d %s\n", CMD_PUSH, prtbuff->part_buffer[i]);
            }
            else if (!strcmp(cmd, "pop")) {
                fprintf(code, "%d\n", CMD_POP);
            }
            else if (!strcmp(cmd, "add")) {
                fprintf(code, "%d\n", CMD_ADD);
            }
            else if (!strcmp(cmd, "sub")) {
                fprintf(code, "%d\n", CMD_SUB);
            }
            else if (!strcmp(cmd, "mul")) {
                fprintf(code, "%d\n", CMD_MUL);
            }
            else if (!strcmp(cmd, "div")) {
                fprintf(code, "%d\n", CMD_DIV);
            }
            else if (!strcmp(cmd, "sqrt")) {
                fprintf(code, "%d\n", CMD_SQRT);
            }
            else if (!strcmp(cmd, "sin")) {
                fprintf(code, "%d\n", CMD_SIN);
            }
            else if (!strcmp(cmd, "cos")) {
                fprintf(code, "%d\n", CMD_COS);
            }
            else {
                fprintf(code, "SNTXERR: '%s'\n", cmd);
            }
        }
    }

    else {
        fprintf(stderr, "file with code did not open(\n");
    }

    if(fclose(code)) {
        fprintf(stderr, "file with code did not close(\n");
    }
    return NO_ERROR;
}