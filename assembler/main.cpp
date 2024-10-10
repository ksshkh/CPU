#include "asm.hpp"

int main(void) {
    Buffer buff = {};
    BuffCtor(&buff);
    // PartitionedBuffer prtbuff = {};
    // PartBuffCtor(&prtbuff, &buff);
    // for(long int i = 0; i < buff.size_of_file; i++) {
    //     printf("%c", buff.buffer[i]);
    // }
    PartitionedBuffer prtbuff = {};
    PartBuffCtor(&prtbuff, &buff);
    // printf("%d\n", count_num_of_symbols(&buff));
    // for(int i = 0; i < prtbuff.size_of_part_buffer; i++) {
    //     printf("%s\n", *(prtbuff.part_buffer + i));
    // }
    AsmRun(&prtbuff);
    return 0;
}