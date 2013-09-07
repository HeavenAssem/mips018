#ifndef MEMORYSYSTEM_H_
#define MEMORYSYSTEM_H_

#include <stdint.h>

class MemorySystem{
	private:

		// Array of memory
		char *MemoryBuffer;

		// size of MemoryBuffer
		int MemorySize;

	public:

		// Default constructor
		// allocates memory size of 64 KB
		// (it will be enough for all)
		MemorySystem();


		// Constructor
		// allocates memory size of MemSize bytes
		// if MemSize > 4GB memory size set as default - 64 KB
		MemorySystem(int MemSize);


		// Method reads 4 bytes on
		// address Address and puts it in data
		// returns true if reading operating was successful
		// else returns false
		bool ReadFromMemory(uint32_t Address, uint32_t &data, int NumberOfBytes);

		// Method writes 4 bytes from data on
		// address Address in MemoryBuffer
		// returns true if writing operating was successful
		// else returns false
		bool WriteToMemory(int Address, int data, int NumberOfBytes);

		// Method writes all memory to stdout
		void LogMemory();

		// Default destructor
		// free memory
		~MemorySystem();
	};


#endif /* MEMORYSYSTEM_H_ */
