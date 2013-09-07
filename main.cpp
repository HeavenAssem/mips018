#include "core.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <stdint.h>
#include <ctype.h>

using namespace std;


void Translate(Core &core,MemorySystem &mem, string filename){
	string raw_string;
    ifstream fin;
    uint32_t addr, instr;
	int first_instr = 0;

    fin.open(filename.c_str(), std::fstream::in);
    getline(fin, raw_string);

    while ( !fin.eof() ){
        if ( isdigit(raw_string.c_str()[0]) ){
            istringstream stream(raw_string);
            stream >> std::hex >> addr >> std::hex >> instr;
            mem.WriteToMemory(addr, instr, 4);
			if (first_instr++ == 0){
				core.set_pc(addr);
			}
            getline(fin, raw_string);
        } else {
            getline(fin, raw_string);
        }
    }
	fin.close();
}


int main(){
	Core core;
	MemorySystem memory(30);
	Translate(core, memory, "/home/vladimir/mips-018/sum.txt");
	core.run_CORE_run(memory);
	memory.LogMemory();
	return 0;
}
