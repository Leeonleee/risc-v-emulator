#include "initialisation.h"

int load_memory(const char *filename, uint8_t *instruction_mem, uint8_t *data_mem) {
    FILE *file = fopen(filename, "rb");

    if (file == NULL) {
        fprintf(stderr, "Memory image binary does not exist\n");
        return 1;
    }
    unsigned char buffer[FILE_SIZE];
    size_t bytesRead = fread(buffer, 1, FILE_SIZE, file);
    fclose(file);

    if (bytesRead != FILE_SIZE) {
        fprintf(stderr, "Error reading 2048 bytes from file\n");
        return 1;
    }

    // Loading instruction memory
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        instruction_mem[i] = buffer[i];
    }

    // Loading data memory
    for (size_t i = MEMORY_SIZE; i < 2 * MEMORY_SIZE; i++) {
        data_mem[i - MEMORY_SIZE] = buffer[i];
    }
    return 0;
}

int initialise_heap_banks(VM *vm) {
    for (int i = 0; i < NUM_HEAP_BANK; i++) {
        vm->heap_banks[i].size = HEAP_BANK_SIZE;
        vm->heap_banks[i].is_allocated = false;
        vm->heap_banks[i].is_head = false;
        vm->heap_banks[i].banks_ahead_in_chunk = 0;
    }
    return 0;
}