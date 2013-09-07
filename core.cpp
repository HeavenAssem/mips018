#include "core.h"
#include "handlers.h"
#include <iostream>
#include <cstdio>
#include <iomanip>


void Core::set_pc(uint32_t new_pc){
        pc = new_pc;
}

uint32_t Core::get_pc(){
        return pc;
}

void Core::set_halt(bool new_halt){
        halt = new_halt;
}

bool Core::get_halt(){
        return halt;
}

void Core::set_register(int Register, uint32_t Value){
        if ( Register>=0 && Register<32 ){
                reg_file[Register-1] = Value;
        }
}

uint32_t Core::get_register(int Register){
        if ( Register>=0 && Register<32){
                return reg_file[Register-1];
        }else {
                return 0;
        }
}

// reading operands of the instruction
void Core::ReadOperands(DecodedInstr &buf, Operands &operands)
{
        switch(buf.instr_format)
        {
        case format_R:
                operands.op1 = get_register(buf.rs);
                operands.op2 = get_register(buf.rt);
                break;
        case format_I:
                operands.op1 = get_register(buf.rs);
        operands.result = get_register(buf.rt);
        operands.immediate = buf.immediate;
                break;
    case format_J:
        operands.address = buf.address;
        break;
        }
}


bool Core::Decode(uint32_t instr, DecodedInstr &buf)
{
    
        uint8_t code = buf.code = instr >> code_offset; // shifting for reading instruction format
        if (code == 0){
      buf.instr_format = format_R;
      buf.rs = (instr >> rs_offset) & reg_mask;
      buf.rt = (instr >> rt_offset) & reg_mask;
      buf.rd = (instr >> rd_offset) & reg_mask;
      buf.func = instr & func_mask;
      return true;
  } else if ( code>=5 && code <=35){
      buf.instr_format = format_I;
      buf.func = 0;
      buf.rs = (instr >> rs_offset) & reg_mask;
      buf.rt = (instr >> rt_offset) & reg_mask;
      buf.immediate = instr & immediate_mask;
      return true;
  } else if ( code == 2 || code == 3 ) {
      buf.instr_format = format_J;
      buf.address = instr & address_mask;
      return true;
  } else {
            return false;       // true - if instr_format was read
  }
}

bool Core::Exception(EXCEPTIONS msg){
        set_halt(false);
        return true;
}


bool Core::Execute(DecodedInstr &buf, Operands &operands){
        uint16_t position;
        position = (buf.code << 6 ) | buf.func;
    
    std::cout<<"Executing instruction " << Handler[position].mnemonic <<std::endl;

        if (!Handler[position].func(*this, buf, operands)){
                Exception(FAIL_EXECUTE);
                return false;
        }
        set_pc(pc+4);
        return true;
}

void Core::WriteResult(Operands &operand, DecodedInstr &buf)
{
        switch(buf.instr_format)
        {
        case format_R:
                set_register(buf.rd, operand.result);
                break;
        case format_I:
                set_register(buf.rt, operand.result);
                break;
        }
}

Core::Core(){
        set_halt(true);
    delay1=delay2=false;
	delay3=delay4=false;
    delayed_pc = 0;
        for (int i=0;i<32;i++){
                set_register(i, 0);
        }
        set_pc(0);

}

void Core::delay_branch(uint32_t addr){
    delay1 = true;
    delayed_pc = addr;
}

void Core::delay_load(uint32_t data, uint8_t r_t)
{
	delay3 = true;
	delayed_data = data;
	delayed_r_t = r_t;
}


    


bool Core::run_CORE_run(MemorySystem &memory)
{
        uint32_t instr;
        DecodedInstr buf;
        Operands operand;

        while(get_halt()){
        
        if (delay2){
            set_pc(delayed_pc);
            delay2 = false;
            delay1 = false;
        }
        delay2 = delay1;
		if (delay4)
		{
			set_register(delayed_r_t, delayed_data);
			delay3 = false;
			delay4 = false;
		}
		delay4 = delay3;
                if(!memory.ReadFromMemory(pc, instr, 4))
                {
            std::cout<<"Reading from memory error"<<std::endl;
                        set_halt(false);
                        Exception(INSTRUCTION_PAGEFAULT);
                        return true;
                }

                if(!Decode(instr, buf))
                {
            std::cout<<"Decidong error"<<std::endl;
                        set_halt(false);
                        Exception(WRONG_INSTRUCTION);
                        return true;
                }
        
                ReadOperands(buf, operand);

                if(!Execute(buf, operand)){
            std::cout<<"Executing error"<<std::endl;
                        set_halt(false);
                        Exception(FAIL_EXECUTE);
                        return true;
                }

                WriteResult(operand, buf);
        
                set_register(0,0);
        for (int i=0;i<32;i++){
                        std::cout<<std::left<<std::setw(3)<< get_register(i);
                }
        std::cout<<std::endl;
        std::cout<<"PC: "<<get_pc()<<std::endl;
        getchar();
        }
    
        return true;
}