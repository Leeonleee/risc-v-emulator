#include "encoding.h"

int type_r(uint32_t instruction, uint8_t opcode, VM *vm) {
    // rd
    uint32_t mask = 0x1F;
    uint8_t rd = (instruction >> 7) & mask;
    // func3
    mask = 0x7000;
    uint8_t func3 = (instruction & mask) >> 12;
    // rs1
    mask = 0xF8000;
    uint8_t rs1 = (instruction & mask) >> 15;
    // rs2
    mask = 0x1F00000;
    uint8_t rs2 = (instruction & mask) >> 20;
    // func7
    mask = 0xFE000000;
    uint8_t func7 = (instruction & mask) >> 25;

    switch (func7)
    {
        case 0b0000000:
            switch (func3)
            {
                case 0b000:
                    add_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b100:
                    xor_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b110:
                    or_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b111:
                    and_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b001:
                    sll_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b101:
                    srl_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b010:
                    slt_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b011:
                    sltu_inst(instruction, rd, rs1, rs2, vm);
            }
            break;
        case 0b0100000:
            switch (func3)
            {
                case 0b000:
                    sub_inst(instruction, rd, rs1, rs2, vm);
                    break;
                case 0b101:
                    sra_inst(instruction, rd, rs1, rs2, vm);
                    break;
            }
            break;
    }

    return 0;
}

int type_i(uint32_t instruction, uint8_t opcode, VM *vm) {
    // rd
    uint32_t mask = 0x1F;
    uint32_t rd = (instruction >> 7) & mask;
    // func3
    mask = 0x7;
    uint32_t func3 = (instruction >> 12) & mask;
    // rs1
    mask = 0x1F;
    uint32_t rs1 = (instruction >> 15) & mask;
    // imm
    mask = 0xFFF;
    uint32_t unsigned_imm = (instruction >> 20) & mask;
    int32_t signed_imm = ((int32_t)(unsigned_imm << 20)) >> 20;

    switch (opcode)
    {
        case 0b0010011:
            switch(func3)
            {
                case 0b000:
                    addi_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b100:
                    xori_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b110:
                    ori_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b111:
                    andi_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b010:
                    slti_inst(instruction, rd, rs1, signed_imm, vm);
                    break;

            }
            break;
        case 0b0000011:
            switch(func3)
            {
                case 0b000:
                    lb_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b001:
                    lh_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b010:
                    lw_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
                case 0b100:
                    lbu_inst(instruction, rd, rs1, unsigned_imm, vm);
                    break;
                case 0b101:
                    lhu_inst(instruction, rd, rs1, unsigned_imm, vm);
                    break;
            }
            break;
        case 0b1100111:
            switch(func3)
            {
                case 0b000:
                    jalr_inst(instruction, rd, rs1, signed_imm, vm);
                    break;
            }
            break;
    }
    return 0;
}

int type_s(uint32_t instruction, uint8_t opcode, VM *vm) {
    // func3
    uint32_t mask = 0x7;
    uint32_t func3 = (instruction >> 12) & mask;
    // rs1
    mask = 0x1F;
    uint32_t rs1 = (instruction >> 15) & mask;
    // rs2
    uint32_t rs2 = (instruction >> 20) & mask;
    // imm
    uint32_t unsigned_imm = ((instruction >> 25) & 0x7F) << 5 | ((instruction >> 7) & 0x1F);
    int32_t signed_imm = ((int32_t)(unsigned_imm << 20)) >> 20;

    switch(func3)
    {
        case 0b000:
            sb_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        case 0b001:
            sh_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        case 0b010:
            sw_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
    }

    return 0;
}

int type_sb(uint32_t instruction, uint8_t opcode, VM *vm) {
    // func3
    uint32_t mask = 0x7;
    uint32_t func3 = (instruction >> 12) & mask;
    // rs1
    mask = 0x1F;
    uint32_t rs1 = (instruction >> 15) & mask;
    // rs2
    uint32_t rs2 = (instruction >> 20) & mask;
    // imm
    uint32_t imm12 = (instruction >> 31) & 0x1;
    uint32_t imm10to5 = (instruction >> 25) & 0x3F;
    uint32_t imm4to1 = (instruction >> 8) & 0xF;
    uint32_t imm11 = (instruction >> 7) & 0x1;
    // combine sections
    uint32_t unsigned_imm = (imm12 << 12) | (imm10to5 << 5) | (imm11 << 11) | (imm4to1 << 1) ;
    int32_t signed_imm = ((int32_t)(unsigned_imm << 19)) >> 19;

    switch(func3)
    {
        case 0b000:
            beq_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        case 0b001:
            bne_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        case 0b100:
            blt_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        case 0b110:
            bltu_inst(instruction, rs1, rs2, unsigned_imm, vm);
            break;
        case 0b101:
            bge_inst(instruction, rs1, rs2, signed_imm, vm);
            break;
        
        case 0b111:
            bgeu_inst(instruction, rs1, rs2, unsigned_imm, vm);
            break;
    }

    return 0;
}

int type_u(uint32_t instruction, uint8_t opcode, VM *vm) {
    // rd
    uint32_t mask = 0x1F;
    uint32_t rd = (instruction >> 7) & mask;
    // imm
    mask = 0xFFFFF000;
    uint32_t unsigned_imm = (instruction & mask);
    int32_t signed_imm = (int32_t)(unsigned_imm);

    lui_inst(instruction, rd, signed_imm, vm);

    return 0;
}

int type_uj(uint32_t instruction, uint8_t opcode, VM *vm) {
    // rd
    uint32_t mask = 0x1F;
    uint32_t rd = (instruction >> 7) & mask;
    // imm
    uint32_t imm20 = (instruction >> 31) &0x1;
    uint32_t imm10to1 = (instruction >> 21) & 0x3FF;
    uint32_t imm11 = (instruction >> 20) & 0x1;
    uint32_t imm19to12 = (instruction >> 12) & 0xFF;
    uint32_t unsigned_imm = (imm20 << 20) | (imm10to1 << 1) | (imm11 << 11) | (imm19to12 << 12);
    int32_t signed_imm = ((int32_t)(unsigned_imm << 11)) >> 11;
    
    jal_inst(instruction, rd, signed_imm, vm);
    return 0;
}

int get_opcode(uint32_t instruction, VM *vm) {
    uint8_t opcode = (instruction) & 0x7F;

    switch (opcode)
    {
        case 0b0110011:
            type_r(instruction, opcode, vm);
            break;
        
        case 0b0010011:
            type_i(instruction, opcode, vm);
            break;
            
        case 0b0110111:
            type_u(instruction, opcode, vm);
            break;

        case 0b0000011:
            type_i(instruction, opcode, vm);
            break;

        case 0b0100011:
            type_s(instruction, opcode, vm);
            break;
        
        case 0b1100011:
            type_sb(instruction, opcode, vm);
            break;

        case 0b1101111:
            type_uj(instruction, opcode, vm);
            break;

        case 0b1100111:
            type_i(instruction, opcode, vm);
            break;
        default:
            // Not implemented
            printf("Instruction Not Implemented: 0x%08x\n", instruction);
            // Register dump
            register_dump(vm);
            exit(1);
            break;
    }
    return 0;
}