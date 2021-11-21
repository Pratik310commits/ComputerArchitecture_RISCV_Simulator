#pragma once
#include <vector>
#include <string>
#include "packet.h"
class MemoryBlock
{
private:
	std::vector<std::string> space;

public:
    void initialiseMemoryBlock();
    void initialiseMemoryBlock(int noOfBytes);
    std::vector<std::string>* getMemorySpace() { return &(this->space); };
    
};

