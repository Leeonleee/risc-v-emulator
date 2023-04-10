#include "virtual_routines.h"

int check_virtual_routines(uint32_t instruction, uint32_t *address, uint32_t *value, VM *vm) {
    uint32_t new_val = *value;

    if (*address == 0x800) {
        char char_val = (char)(new_val);// & 0xFF
        printf("%c", char_val);
    }
    else if (*address == 0x804) {
        printf("%d", (int32_t)new_val);
    }
    else if(*address == 0x0808) {
        printf("%x", new_val);
    }
    else if (*address == 0x080C) {
        printf("CPU Halt Requested\n");
        exit(0);
    }
    else if (*address == 0x0812) {
        
        char input_char;
        if (scanf("%c", &input_char) != 1) {
            printf("Error reading input.\n");
            exit(1);
        }
        if (input_char >= 0 && input_char <= 127) {
            uint32_t val = (uint32_t)input_char;
            *value = val;
        }
        else {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
            return 1;
        }
    }
    else if (*address == 0x0816) {
        int32_t input_int;
        if (scanf("%d", &input_int) != 1) {
            printf("Error reading input.\n");
            exit(1);
        }
        if (input_int > (2147483647) || input_int < (-2147483648)) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            *value = input_int;
        }
    }
    else if (*address == 0x0820) {
        printf("%x", vm->pc);
    }
    else if (*address == 0x0824) {
        register_dump(vm);
    }
    else if (*address == 0x0828) {
        uint32_t v = *value;
        if (v >= INSTRUCTION_MEMORY_START && v <= INSTRUCTION_MEMORY_END) {
            uint32_t val = (vm->instruction_mem[v] << 0) |
                            (vm->instruction_mem[v + 1] << 8) |
                            (vm->instruction_mem[v + 2] << 16) |
                            (vm->instruction_mem[v + 3] << 24);
            printf("%d", val);
        }
        else if (v >= DATA_MEMORY_START && v <= DATA_MEMORY_END) {
            uint32_t val = (vm->data_mem[v- 1024] << 0) |
                            (vm->data_mem[v - 1023] << 8) |
                            (vm->data_mem[v - 1022] << 16) |
                            (vm->data_mem[v - 1021] << 24); 
            printf("%d", val);
        }
        // heap banks
        else if (v >= HEAP_BANKS_START) {
            printf("Dump Memory Word: Heap Banks");
        }
        else {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        }
    }


    else if (*address == 0x0830) { // malloc
        
        int malloc_size = new_val; // chunk of memory with size malloc_size
        if (malloc_size == 0){
            vm->regs[28] = 0x0; // null pointer
        }
        int banks_needed;
        if ((malloc_size % HEAP_BANK_SIZE ) != 0){
            banks_needed = (malloc_size - (malloc_size % HEAP_BANK_SIZE))/HEAP_BANK_SIZE + 1;
        }else{
            banks_needed = malloc_size / HEAP_BANK_SIZE;
        }

        if (banks_needed > NUM_HEAP_BANK) {
            vm->regs[28] = 0;
            return 0;
        }

        int bank_index = 0;
        bool exit_flag = false;
        while(true){
            exit_flag = false;
            if(bank_index >= HEAP_BANK_SIZE - 1){
                vm->regs[28] = 0;
            }
            for(int i = 0; i < banks_needed; i++){
                if (bank_index + i > NUM_HEAP_BANK - 1) {
                    exit_flag = true;
                    vm->regs[28] = 0x0;
                    return 0;
                }
                HeapBank tmp = vm->heap_banks[bank_index + i];
                if (tmp.is_allocated == true){
                    exit_flag = true;
                }
                if(exit_flag == true){
                    break; //not found, next index.
                }
            }
            if(exit_flag == false){
                break; //found
            }
            bank_index++;
        }


        uint32_t starting_address =  0xb700 + (bank_index * HEAP_BANK_SIZE);
        if (starting_address <= 0xd700) {
            vm->regs[28] = starting_address;
        } else {
            vm->regs[28] = 0;
        }

        for(int i = 0; i < banks_needed; i++){

            if(vm->heap_banks[bank_index + i].is_allocated == false){
                vm->heap_banks[bank_index + i].is_allocated = true;
                if(i == 0){
                    vm->heap_banks[bank_index + i].is_head = true; //head
                }
                vm->heap_banks[bank_index + i].banks_ahead_in_chunk = banks_needed - i - 1;
                if(malloc_size >= HEAP_BANK_SIZE){
                    vm->heap_banks[bank_index + i].size = HEAP_BANK_SIZE;
                    malloc_size = malloc_size - vm->heap_banks[bank_index + i].size;
                }else{
                    vm->heap_banks[bank_index + i].size = malloc_size;
                }
            }else{
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
                // TODO: remove later
            }
        }

        
    }
    else if (*address == 0x0834) { // free
        // free a chunk of memory starting at new_val
        uint32_t starting_address = new_val;
        if (starting_address <= HEAP_BANKS_START || starting_address >= HEAP_BANKS_END) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        }
        if (starting_address % 64 != 0) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        }

        int bank_index = (starting_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
        if (vm->heap_banks[bank_index].is_allocated == false ||
            vm->heap_banks[bank_index].is_head == false) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            //printf("FREEING!\n");
            int i = 0;
            while(true) {
                HeapBank temp = vm->heap_banks[bank_index + i];
                if (temp.banks_ahead_in_chunk == 0) {
                    break;
                }
                if (i == 0) {
                    temp.is_head = false;
                }
                temp.is_allocated = false;
                temp.banks_ahead_in_chunk = 0;
                temp.size = 0;
                for (int j = 0; j < HEAP_BANK_SIZE; j++) {
                    temp.data[j] = 0;
                }
                i++;
            }
        }

    }
    else {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }

    return 0;
}