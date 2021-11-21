#include "memoryBlock.h"

MemoryBlock::MemoryBlock(){}

void MemoryBlock::initialiseMemoryBlock()
{
    this->initialiseMemoryBlock(5120);
}

void MemoryBlock::initialiseMemoryBlock(int noOfBytes)
{
    this->space.resize(noOfBytes, 0);
}

std::bitset<8> *MemoryBlock::getMemoryAddressByIndex(int index)
{
    std::vector <std::bitset<8>>::iterator itr = this->space.begin();
    std::bitset<8> * ptr = &(*itr);

    return ptr + index;
}

std::bitset<8> MemoryBlock::getDataByAddress(std::bitset<8> *addressPtr)
{
    return *addressPtr;
}

void MemoryBlock::setDataByAddress(std::bitset<8> *addressPtr, std::bitset<8> data)
{
    *addressPtr = data;
}

void MemoryBlock::getDataByBlock(int blockSizeInBytes, int blockIndex, BigPacket &packet)
{
    //divide block size by 8, this will give number of bytes in single block = sb
    //bi * sb, this is the index
    //use the index to retrieve total sb bytes
    int bytesInSingleBlock = blockSizeInBytes/8;
    int memoryIndex = blockIndex*bytesInSingleBlock;
    std::vector<std::bitset<8>> data;
    
    for(int itr = 0; itr < bytesInSingleBlock; ++itr)
    {
        data.push_back(this->space.at(memoryIndex + itr));
    }
    packet.populateBlockData(data);
}