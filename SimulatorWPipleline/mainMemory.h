#pragma once
#include "memoryBlock.h"
class MainMemory :
	public MemoryBlock
{
public:
	std::string name;
	//std::bitset<13> getMemoryAddressByIndex(int index);
	void getDataByAddress(std::string address, int dataSizeInBytes, Packet& packet);
	void setDataByAddress(std::string address, int dataSizeInBytes, Packet packet);
	void getDataByBlock(int blockSizeInBytes, int blockIndex, Packet& packet);
	void setDataByBlock(int blockSizeInBytes, int blockIndex, Packet packet);
};

