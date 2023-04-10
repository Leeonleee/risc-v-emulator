#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


#include <stdint.h>
#include <stdio.h>
#include "vm.h"
#include "virtual_routines.h"

// Arithmetic and Logic Operations
int add_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int addi_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sub_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int lui_inst(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm);
int xor_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int xori_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int or_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int ori_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int and_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int andi_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sll_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int srl_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int sra_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
// Memory Access Operations
int lb_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lh_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lw_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lbu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int lhu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sb_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int sh_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int sw_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
// Program Flow Operations
int slt_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int slti_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int sltu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t rs2, VM *vm);
int sltiu_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
int beq_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bne_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int blt_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bge_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bltu_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int bgeu_inst(uint32_t instruction, uint32_t rs1, uint32_t rs2, uint32_t imm, VM *vm);
int jal_inst(uint32_t instruction, uint32_t rd, uint32_t imm, VM *vm);
int jalr_inst(uint32_t instruction, uint32_t rd, uint32_t rs1, uint32_t imm, VM *vm);
#endif