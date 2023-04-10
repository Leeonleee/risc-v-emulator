#include "instructions.h"

// Arithmetic and Logic Operations
int add_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] + R[rs2]
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] + vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int addi_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = R[rs1] + imm
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] + imm;
    }
    vm->pc += 4;
    return 0;
}

int sub_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] - R[rs2]
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] - vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int lui_inst(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm) {
    // R[rd] = imm << 12
    if (rd != 0) {
        vm->regs[rd] = (imm & 0xFFFFF000);
    }
    vm->pc += 4;
    return 0;
}

int xor_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] ^ R[rs2]
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] ^ vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int xori_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = R[rs1] ^ imm
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] ^ imm;
    }
    vm->pc += 4;
    return 0;
}

int or_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] | R[rs2]
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] | vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int ori_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = R[rs1] | imm
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] | imm;
    }
    vm->pc += 4;
    return 0;
}

int and_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] & R[rs2]
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] & vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int andi_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = R[rs1] & imm
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] & imm;
    }
    vm->pc += 4;
    return 0;
}

int sll_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] << R[rs2]
    if (vm->regs[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] << vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int srl_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] >> R[rs2]
    if (vm->regs[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->regs[rd] = vm->regs[rs1] >> vm->regs[rs2];
    }
    vm->pc += 4;
    return 0;
}

int sra_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = R[rs1] >> R[rs2]
    if (vm->regs[rs2] > 31) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    if (rd != 0) {
        vm->regs[rd] = (vm->regs[rs1] >> vm->regs[rs2]) | (rs1 << (32 - vm->regs[rs2]));
    }
    vm->pc += 4;
    return 0;
}

// Memory Access Operations

int lb_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = sext(M[R[rs1] + imm])
    if (rd != 0) {
        uint32_t memory_address = vm->regs[rs1] + imm;
        
        if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
            uint8_t byte_val = vm->instruction_mem[memory_address];
            int32_t sign_extended_val = (int32_t)(int8_t)byte_val;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
            uint8_t byte_val = vm->data_mem[memory_address - 1024];
            int32_t sign_extended_val = (int32_t)(int8_t)byte_val;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            int32_t sign_extended_val = (int32_t)(int8_t)val;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
            if (vm->heap_banks[bank_index].is_allocated == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
                uint8_t byte_val = vm->heap_banks[bank_index].data[data_index];
                int32_t sign_extended_val = (int32_t)(int8_t)byte_val;
                vm->regs[rd] = sign_extended_val;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lh_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = sext(M[R[rs1] + imm])
    if (rd != 0) {
        //vm->regs[rd] = (int16_t)vm->mem[vm->regs[rs1] + imm];
        uint32_t memory_address = vm->regs[rs1] + imm;
        
        if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
            int16_t two_byte_val = (vm->instruction_mem[memory_address] << 0) | (vm->instruction_mem[memory_address + 1] << 8);
            int32_t sign_extended_val = (two_byte_val << 16) >> 16;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
            int16_t two_byte_val = (vm->data_mem[memory_address- 1024] << 0) | (vm->data_mem[memory_address - 1023] << 8);
            int32_t sign_extended_val = (two_byte_val << 16) >> 16;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            int32_t sign_extended_val = (int32_t)(int8_t)val;
            vm->regs[rd] = sign_extended_val;
        }
        if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            //printf("Loading from heap bank\n");

            int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
            if (vm->heap_banks[bank_index].is_allocated == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
                uint16_t two_byte_val = (vm->heap_banks[bank_index].data[data_index] << 0) |
                                        (vm->heap_banks[bank_index].data[data_index + 1] << 8);
                int32_t sign_extended_val = (int32_t)(int16_t)two_byte_val;
                vm->regs[rd] = sign_extended_val;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lw_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = M[R[rs1] + imm]
    if (rd != 0) {
        uint32_t memory_address = vm->regs[rs1] + imm;
        if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
            int32_t val = (vm->instruction_mem[memory_address] << 0) |
                            (vm->instruction_mem[memory_address + 1] << 8) |
                            (vm->instruction_mem[memory_address + 2] << 16) |
                            (vm->instruction_mem[memory_address + 3] << 24);
            vm->regs[rd] = val;
        }
        if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
            int32_t val = (vm->data_mem[memory_address- 1024] << 0) |
                            (vm->data_mem[memory_address - 1023] << 8) |
                            (vm->data_mem[memory_address - 1022] << 16) |
                            (vm->data_mem[memory_address - 1021] << 24);            
            vm->regs[rd] = val;
        }
        if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            vm->regs[rd] = val;
        }
        if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            
            int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
            if (vm->heap_banks[bank_index].is_allocated == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
                int32_t val = (vm->heap_banks[bank_index].data[data_index] << 0) |
                                (vm->heap_banks[bank_index].data[data_index + 1] << 8) |
                                (vm->heap_banks[bank_index].data[data_index + 2] << 16) |
                                (vm->heap_banks[bank_index].data[data_index + 3] << 24);
                vm->regs[rd] = val;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lbu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = M[R[rs1] = imm]
    if (rd != 0) {
        uint32_t memory_address = vm->regs[rs1] + imm;
        if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
            uint8_t byte_val = vm->instruction_mem[memory_address];
            vm->regs[rd] = (uint32_t)byte_val;
        }
        if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
            uint8_t byte_val = vm->data_mem[memory_address - 1024]; // FIXME: causing the no print error
            vm->regs[rd] = (uint32_t)byte_val;
        }
        if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            vm->regs[rd] = val;
        }
        if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
            if (vm->heap_banks[bank_index].is_allocated == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
                uint8_t byte_val = vm->heap_banks[bank_index].data[data_index];
                vm->regs[rd] = (uint32_t)byte_val;
            }
        }
    }
    vm->pc += 4;
    return 0;
}

int lhu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = M[R[rs1] + imm]
    if (rd != 0) {
        uint32_t memory_address = vm->regs[rs1] + imm;
        if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
            uint16_t two_byte_val = (vm->instruction_mem[memory_address] << 0) | (vm->instruction_mem[memory_address + 1] << 8);
            vm->regs[rd] = (uint32_t)two_byte_val;
        }
        if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
            uint16_t two_byte_val = (vm->data_mem[memory_address- 1024] << 0) | (vm->instruction_mem[memory_address - 1023] << 8);
            vm->regs[rd] = (uint32_t)two_byte_val;
        }
        if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
            uint32_t val;
            check_virtual_routines(instruction, &memory_address, &val, vm);
            vm->regs[rd] = val;
        }
        if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
            int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
            if (vm->heap_banks[bank_index].is_allocated == false) {
                printf("Illegal Operation: 0x%08x\n", instruction);
                register_dump(vm);
                exit(1);
            } else {
                int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
                uint16_t two_byte_val = (vm->heap_banks[bank_index].data[data_index] << 0) |
                                        (vm->heap_banks[bank_index].data[data_index + 1] << 8);
                vm->regs[rd] = (uint32_t)two_byte_val;
            }
        }
    }
    return 0;
}

int sb_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // M[R[rs1] + imm] = R[rs2]
    uint32_t memory_address = vm->regs[rs1] + imm;
    if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    else if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
        vm->data_mem[memory_address - 1024] = (uint8_t)vm->regs[rs2];
    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->regs[rs2], vm);
    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
        //printf("Storing to heap bank\n");
        int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
        if (vm->heap_banks[bank_index].is_allocated == false) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
            vm->heap_banks[bank_index].data[data_index] = (uint8_t)vm->regs[rs2];
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

int sh_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // M[R[rs1] + imm] = R[rs2]
    uint32_t memory_address = vm->regs[rs1] + imm;
    if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    else if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
        uint32_t value = vm->regs[rs2];
        uint8_t byte0 = value & 0xFF;
        uint8_t byte1 = (value >> 8) & 0xFF;

        vm->data_mem[memory_address - 1024] = byte0;
        vm->data_mem[memory_address - 1023] = byte1;

    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->regs[rs2], vm);

    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
        //printf("Storing to heap bank\n");
        int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
        if (vm->heap_banks[bank_index].is_allocated == false) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
            uint32_t value = vm->regs[rs2];
            uint8_t byte0 = value & 0xFF;
            uint8_t byte1 = (value >> 8) & 0xFF;

            vm->heap_banks[bank_index].data[data_index] = byte0;
            vm->heap_banks[bank_index].data[data_index + 1] = byte1;
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

int sw_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // M[R[rs1] + imm] = R[rs2]

    

    uint32_t memory_address = vm->regs[rs1] + imm;


    if (memory_address >= INSTRUCTION_MEMORY_START && memory_address <= INSTRUCTION_MEMORY_END) {
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
        return 1;
    }
    else if (memory_address >= DATA_MEMORY_START && memory_address <= DATA_MEMORY_END) {
        uint32_t value = vm->regs[rs2];
        uint8_t byte0 = value & 0xFF;
        uint8_t byte1 = (value >> 8) & 0xFF;
        uint8_t byte2 = (value >> 16) & 0xFF;
        uint8_t byte3 = (value >> 24) & 0xFF;

        vm->data_mem[memory_address - 1024] = byte0;
        vm->data_mem[memory_address - 1023] = byte1;
        vm->data_mem[memory_address - 1022] = byte2;
        vm->data_mem[memory_address - 1021] = byte3;
    }
    else if (memory_address >= VIRTUAL_ROUTINES_START && memory_address <= VIRTUAL_ROUTINES_END) {
        check_virtual_routines(instruction, &memory_address, &vm->regs[rs2], vm);

    }
    else if (memory_address >= HEAP_BANKS_START && memory_address <= HEAP_BANKS_END) {
        int bank_index = (memory_address - HEAP_BANKS_START)/HEAP_BANK_SIZE;
        if (vm->heap_banks[bank_index].is_allocated == false) {
            printf("Illegal Operation: 0x%08x\n", instruction);
            register_dump(vm);
            exit(1);
        } else {
            int data_index = (memory_address - (HEAP_BANKS_START +( bank_index * HEAP_BANK_SIZE)));
            uint32_t value = vm->regs[rs2];
            uint8_t byte0 = value & 0xFF;
            uint8_t byte1 = (value >> 8) & 0xFF;
            uint8_t byte2 = (value >> 16) & 0xFF;
            uint8_t byte3 = (value >> 24) & 0xFF;
            vm->heap_banks[bank_index].data[data_index] = byte0;
            vm->heap_banks[bank_index].data[data_index + 1] = byte1;
            vm->heap_banks[bank_index].data[data_index + 2] = byte2;
            vm->heap_banks[bank_index].data[data_index + 3] = byte3;
            
        }
    } else{
        printf("Illegal Operation: 0x%08x\n", instruction);
        register_dump(vm);
        exit(1);
    }
    vm->pc += 4;
    return 0;
}

// Program Flow Operations

int slt_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
    if (rd != 0) {
        if ((int32_t)vm->regs[rs1] < (int32_t)vm->regs[rs2]) {
            vm->regs[rd] = 1;
        } else {
            vm->regs[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int slti_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = (R[rs1] < imm) ? 1 : 0
    if (rd != 0) {
        if ((int32_t)vm->regs[rs1] < (int32_t)imm) {
            vm->regs[rd] = 1;
        } else {
            vm->regs[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int sltu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm) {
    // R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
    if (rd != 0) {
        if (vm->regs[rs1] < vm->regs[rs2]) {
            vm->regs[rd] = 1;
        } else {
            vm->regs[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int sltiu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = (R[rs1] < imm) ? 1 : 0
    if (rd != 0) {
        if (vm->regs[rs1] < imm) {
            vm->regs[rd] = 1;
        } else {
            vm->regs[rd] = 0;
        }
    }
    vm->pc += 4;
    return 0;
}

int beq_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] == R[rs2]) then pc += imm
    if ((int32_t)vm->regs[rs1] == (int32_t)vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int bne_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] != R[rs2]) then pc += imm
    if ((int32_t)vm->regs[rs1] != (int32_t)vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int blt_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] < R[rs2]) then pc += imm
    if ((int32_t)vm->regs[rs1] < (int32_t)vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int bltu_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] < R[rs2]) then pc += imm
    if (vm->regs[rs1] < vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int bge_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] >= R[rs2]) then pc += imm
    if ((int32_t)vm->regs[rs1] >= (int32_t)vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int bgeu_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm) {
    // if (R[rs1] >= R[rs2]) then pc += imm
    if (vm->regs[rs1] >= vm->regs[rs2]) {
        vm->pc = vm->pc + imm;
    } else {
        vm->pc += 4;
    }
    return 0;
}

int jal_inst(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm) {
    // R[rd] = pc + 4
    // pc += imm
    if (rd != 0) {
        vm->regs[rd] = vm->pc + 4;
    }
    vm->pc = vm->pc + imm;
    return 0;
}

int jalr_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm) {
    // R[rd] = pc + 4
    // pc = R[rs1] + imm
    if (rd != 0) {
        vm->regs[rd] = vm->pc + 4;
    }
    vm->pc = vm->regs[rs1] + imm;
    return 0;
}