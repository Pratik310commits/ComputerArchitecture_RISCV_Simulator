#ifndef _MEMORY_BLOCK_H
#define _MEMORY_BLOCK_H

#include <bitset>
#include <vector>
#include "packet.h"
#include "bigPacket.h"

class MemoryBlock
{
    private:
    std::vector<std::bitset<8>> space;
    public:    
    MemoryBlock();
    
    void initialiseMemoryBlock();
    void initialiseMemoryBlock(int noOfBytes);
    std::bitset<8> *getMemoryAddressByIndex(int index);
    std::bitset<8> getDataByAddress(std::bitset<8> *addressPtr);
    void setDataByAddress(std::bitset<8> *addressPtr, std::bitset<8> data);
    void getDataByBlock(int blockSizeInBytes, int blockIndex, BigPacket &packet);
};

#endif