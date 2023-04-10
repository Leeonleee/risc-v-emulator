#ifndef VM_H
#define VM_H

#include <stdint.h>
#include <stdbool.h>

#define INSTRUCTION_MEMORY_START    0x0000
#define INSTRUCTION_MEMORY_END      0x3FF
#define DATA_MEMORY_START           0x0400
#define DATA_MEMORY_END             0x7FF
#define VIRTUAL_ROUTINES_START      0x0800
#define VIRTUAL_ROUTINES_END        0x8FF
#define HEAP_BANKS_START            0xB700
#define HEAP_BANKS_END              0xD700

#define FILE_SIZE 2048
#define MEMORY_SIZE 1024
#define NUM_ELEM (MEMORY_SIZE / sizeof(uint32_t))
#define NUM_REGISTERS 32
#define NUM_HEAP_BANK 128
#define HEAP_BANK_SIZE 64

// Heap Banks
typedef struct HeapBank{
    uint32_t size;
    uint8_t data[HEAP_BANK_SIZE];
    bool is_allocated;
    bool is_head;
    int banks_ahead_in_chunk;
} HeapBank;

typedef struct {
    int count;
    uint32_t pc;
    uint32_t regs[NUM_REGISTERS];
    uint8_t instruction_mem[MEMORY_SIZE];
    uint8_t data_mem[MEMORY_SIZE];
    HeapBank heap_banks[NUM_HEAP_BANK];
} VM;

#endif