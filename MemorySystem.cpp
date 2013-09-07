#include "MemorySystem.h"
#include <iostream>
#include <iomanip>

MemorySystem::MemorySystem(){
	MemorySize = 65536;
	MemoryBuffer = new char[MemorySize];
}


MemorySystem::MemorySystem(int MemSize){
	if (MemSize <= 4294967296){
		MemorySize = MemSize;
		MemoryBuffer = new char[MemorySize];
	}
}

MemorySystem::~MemorySystem(){
	delete [] MemoryBuffer;
	MemoryBuffer = NULL;
}

bool MemorySystem::ReadFromMemory(uint32_t Address, uint32_t &data, int NumberOfBytes){
	if ((Address + NumberOfBytes < MemorySize) && (NumberOfBytes%2==0 || NumberOfBytes==1)){
		data = 0;
		for (int i = 0; i<NumberOfBytes; i++){
			data = data << 8;
			data = data | ((unsigned)MemoryBuffer[Address+i] & 0xFF);
		}
		return true;
	} else {
		return false;
	}
}

bool MemorySystem::WriteToMemory(int Address, int data, int NumberOfBytes){
	int _data = 0;
	if ((Address + NumberOfBytes < MemorySize) && (NumberOfBytes%2==0 || NumberOfBytes==1)){
		for (int i = 0; i<NumberOfBytes; i++){
			_data = data>>((NumberOfBytes - i - 1)*8) ;
			MemoryBuffer[Address+i] = _data;
		}
		return true;
	} else {
		return false;
	}
}

void MemorySystem::LogMemory(){
	for (int i = 0; i < MemorySize;i++){
		std::cout<< std::left<<std::setw(5)<< (int)MemoryBuffer[i];
	}
    std::cout<<std::endl;
}
