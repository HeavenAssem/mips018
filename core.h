#ifndef CORE_H
#define CORE_H

#include "MemorySystem.h"
#include <stdint.h>


#define R 0		// it's meant R-type instruction

#define I_ADDI   0x8   // it's meant I-type instructions
#define I_ADDIU  0x9  
#define I_SLTI   0xA
#define I_SLTIU  0xB
#define I_ANDI   0xC
#define I_ORI    0xD
#define I_XORI   0xE

#define J 2		// it's meant J-type instruction(jump, actually)
#define JL 3    // it's meant J-type instruction(jump and link, actually)
#define format_R 0		// convenient form of format R
#define format_I 1		// convenient form of format I
#define format_J 2		// convenient form of format J

#define address_mask   0x3FFFFF // mask to extract address from J-type instruction
#define immediate_mask 0xFFFF   // mask to extract immediate value from I-type instruction
#define reg_mask       0x1F     // mask to extract register number from instruction
#define func_mask      0x3F     // mask to extract function code from R-type instruction
#define code_offset    0x1A     // offset to extract instruction code from instruction
#define rs_offset      0x15     // use with reg_mask to extract number of rs register
#define rt_offset      0x10     // use with reg_mask to extract number of rt register
#define rd_offset      0xB      // use with reg_mask to extract number of rd register


enum EXCEPTIONS{INSTRUCTION_PAGEFAULT, WRONG_INSTRUCTION, INT_OVERFLOW, HALT, FAIL_EXECUTE};


// structure that describes instruction
struct DecodedInstr
{
	uint8_t rs;		// register for first argument
	uint8_t rt;		// register for second argument
	uint8_t rd;		// register, where the result is written
	uint8_t shamt;  // ya ne znau, chto eto takoe
	uint8_t func;   // function code
	uint8_t code;   // instruction code
	uint16_t immediate;		// number(second operand to I-type instructions)
	uint8_t instr_format;	// instruction format (R, I or J)
	uint32_t instr_attributes; // kakie-to atributy.
	uint32_t address;          // address for J-type instruction
};
struct Operands
{
	uint32_t op1;	// first operand
	uint32_t op2;	// second operand
	uint32_t result;	// result of instruction
	uint32_t address;   // address for J-type instruction
	uint16_t immediate;		// operand for immediate-type instruction
};

class Core
{
	uint32_t reg_file[32];	// register file
	uint32_t pc;	//Program counter
	bool halt;		// this properties is responsible for executing next instruction
    bool delay1;
    bool delay2;
    bool delay3;
    bool delay4;
    uint32_t delayed_data;
    uint8_t delayed_r_t;
    uint32_t delayed_pc;
public:

	// Constructor for initializing some Core properties
	Core();
    
    void delay_branch(uint32_t addr);
    void delay_load(uint32_t addr, uint8_t r);

	void set_halt(bool new_halt);

	bool get_halt();

	void set_pc(uint32_t new_pc);

	uint32_t get_pc();

	void set_register(int Register, uint32_t Value);

	uint32_t get_register(int Register);

	// This function process exceptions
	bool Exception(EXCEPTIONS);

	// decoding of the instruction's type
	bool Decode(uint32_t instr, DecodedInstr &buf);

	// reading operands
	void ReadOperands(DecodedInstr &buf, Operands &operands);
	// executing the instruction
	bool Execute(DecodedInstr &buf, Operands &operands);

	// write results in appropriate register
	void WriteResult(Operands &operand, DecodedInstr &buf);

	// main core running cycle
	bool run_CORE_run(MemorySystem &memory);
};



#endif
