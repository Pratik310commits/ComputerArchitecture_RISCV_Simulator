#include "memoryBlock.h"

void MemoryBlock::initialiseMemoryBlock()
{
	this->initialiseMemoryBlock(5120);
}

void MemoryBlock::initialiseMemoryBlock(int noOfBytes)
{
	this->space.resize(noOfBytes, "");
}




