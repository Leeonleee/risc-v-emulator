#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "vm.h"
#include "initialisation.h"
#include "instructions.h"
#include "encoding.h"
#include "virtual_routines.h"
#include "register_dump.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "No arguments given\n");
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "Too many arguments given\n");
        return 1;
    }

    const char *filename = argv[1];
    VM vm = {0}; // initialise vm with 0
    //VM vm;
    vm.pc = 0;

    if (load_memory(filename, vm.instruction_mem, vm.data_mem) == 1) {
        exit(1);
    }
    initialise_heap_banks(&vm);  

    while(true){
        uint8_t *instruction_pointer = (uint8_t*)vm.instruction_mem;
        uint32_t instruction = *(uint32_t*)(instruction_pointer + vm.pc);
        vm.regs[0] = 0;
        get_opcode(instruction, &vm);

    }

    return 0;
}