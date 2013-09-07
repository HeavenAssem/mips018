#include <stdint.h>
#include <iostream>
#include "core.h"
#include "MemorySystem.h"


bool func_none( Core &core, const DecodedInstr &instr, Operands &ops ){
	return false;
}

// function ADD (overflow trap)
// code = 0x0
// function number = 0x20
// adds two registers and stores the result in a register
// returns true if result is right
// else returns false
bool func_add( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ops.op1 + ops.op2;
    if ( ((~ops.op1)&(~ops.op2)&(ops.result)) | ((ops.op1)&(ops.op2)&(~ops.result)) >> 31 ){
        core.Exception(INT_OVERFLOW);
        return false;
    }else {
        return true;
    }
}

// function ADDI (overflow trap)
// code = 0x8
// function number = 0x0
// Adds a register and a sign-extended immediate value
// and stores the result in a register
// returns true if result is right
// else returns false
bool func_addi( Core &core, const DecodedInstr &instr, Operands &ops){
    ops.result = ops.op1 + ops.immediate;
    if ( ((~ops.op1)&(ops.result)) | ((ops.op1)&(~ops.result)) >> 31 ){
        core.Exception(INT_OVERFLOW);
        return false;
    } else {
        return true;
    }
}

// function ADDIU (no overflow)
// code = 0x9
// function number = 0x0
// Adds a register and a sign-extended immediate value
// and stores the result in a register
// returns true
bool func_addiu( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ops.op1 + ops.immediate;
    return true;
}

// function ADDU (overflow trap)
// code = 0x0
// function number = 0x21
// adds two registers and stores the result in a register
// returns true
bool func_addu( Core &core, const DecodedInstr &instr, Operands &ops){
    ops.result = ops.op1 + ops.op2;
    return true;
}

// function SUB
// code = 0x0
// function number = 0x22
// subtracts two register and stores the result in a register
// returns true
bool func_sub( Core &core, const DecodedInstr &instr, Operands &ops){
    ops.result = ops.op1 - ops.op2;
    return true;
}


// function SUBU
// code = 0x0
// function number = 0x23
// subtracts two register and stores the result in a register
// returns true
bool func_subu( Core &core, const DecodedInstr &instr, Operands &ops){
    ops.result = ops.op1 - ops.op2;
    return true;
}

// function AND
// code = 0x0
// function number = 0x24
// bitwise ands two registers and stores the result in a register
// returns true
bool func_and( Core &core, const DecodedInstr &instr, Operands &ops){
    ops.result = ops.op1 & ops.op2;
    return true;
}

// function ANDI
// code = 0xC
// function number = 0x0
// bitwise ands a register and an immediate value and stores the result in a register
// returns true
bool func_andi( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ops.op1 & ops.immediate;
    return true;
}

// function OR
// code = 0x0
// function number = 0x25
// bitwise logical ors two registers and stores the result in a register
// returns true
bool func_or( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ops.op1 | ops.op2;
    return true;
}

// function ORI
// code = 0xD
// function number = 0x0
// bitwise logical ors register and immediate value and stores the result in a register
// returns true
bool func_ori( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ops.op1 | ops.immediate;
    return true;
}

// function NOR
// code = 0x0
// function number = 0x27
// bitwise logical nors two registers and stores the result in a register
// returns true
bool func_nor( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = ~ (ops.op1 | ops.op2);
    return true;
}

// function SLT
// code = 0x0
// function number = 0x2A
// If $s is less than $t, $d is set to one. It gets zero otherwise
// returns true
bool func_slt( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = (ops.op1 < ops.op2) ? 1 : 0;
    return true;
}

// function SLTI
// code = 0xA
// function number = 0x0
// If $s is less than immediate, $t is set to one. It gets zero otherwise
// returns true
bool func_slti( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result /* or ops.op2 ? */ = (ops.op1 < ops.immediate) ? 1 : 0;
    return true;
}

// function SLTIU
// code = 0xB
// function number = 0x0
// If $s is less than immediate, $t is set to one. It gets zero otherwise
// returns true
bool func_sltiu( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result /* or ops.op2 ? */ = (ops.op1 < ops.immediate) ? 1 : 0;
    return true;
}

// function SLTU
// code = 0x0
// function number = 0x2B
// If $s is less than $t, $d is set to one. It gets zero otherwise
// returns true
bool func_sltu( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.result = (ops.op1 < ops.op2) ? 1 : 0;
    return true;
}

// function LHI
// code = 0x19
// function number = 0x0
// sets immediate at low half of a register
// returns true
bool func_lhi( Core &core, const DecodedInstr &instr, Operands &ops ){
    ops.op1 = ops.immediate;
    return true;
}

// function LLO
// code = 0x20
// function number = 0x0
// sets immediate at high half of a register
// returns true
bool func_llo( Core &core, const DecodedInstr &instr, Operands &ops ){
    int i;
    uint32_t bit = 0;
    ops.op1 = 0;
    for ( i=0; i<16; i++){
        ops.op1 = ops.op1 >> 1;
        if ( (bit = ops.immediate & 1) == 1){
            ops.op1 = ops.op1 | bit<<31;
        }
        ops.immediate = ops.immediate >> 1;
    }
    return true;
}

// function J (Jump)
// code = 0x1
// function number = 0x0
// jumps to the calculated address
bool func_j( Core &core, const DecodedInstr &instr, Operands &ops){
    if (ops.address != 0){
        core.set_pc(core.get_pc() + ops.address);
        return true;
    } else {
        return false;
    }
}

// function BNE (Branch on not equal)
// code = 0x5
// function number = 0x0
// branches if the two registers are not equal
bool func_bne( Core &core, const DecodedInstr &instr, Operands &ops){
    std::cout<<"op1: "<<ops.op1<<" result: "<<ops.result<<std::endl;
    std::cout<<"BNE immediate: "<<ops.immediate<<std::endl;
    if (ops.op1!=ops.result){
        core.delay_branch(core.get_pc() + (int32_t)(int16_t)ops.immediate);
    }
    return true;
}
// load word(32-bit)
bool func_lw(Core &core, const DecodedInstr &instr, Operands &ops){
	uint32_t data;

	if(!ReadFromMemory(ops.op2 + (int32_t)(int16_t)ops.immediate, data, 4))
	{
		core.Exception(INSTRUCTION_PAGEFAULT);
		return false;
	}
	else
	{
		core.delay_load(data, instr.rt);
		return true;
	}
}
