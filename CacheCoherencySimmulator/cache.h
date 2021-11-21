#ifndef _CACHE_H
#define _CACHE_H

#include <bitset>
#include <vector>
#include <math.h>
#include <iostream>
#include "memoryBlock.h"
#include "bigPacket.h"
#include "port.h"

class Cache
{
    private:
    std::vector<std::vector<std::bitset<8>>> space;
    int cacheLineSizeInBytes;
    int cacheSizeInBytes;
    std::vector<std::string> tagBlock;
    int essentialTagBits;
    int sizeOfConnectedMemoryInBytes;
    MemoryBlock *memoryConnection;
    Port memoryPort;
    //Port controllerPort;
    std::vector<int> lruBlock;
    std::vector<bool> dirtyBitRegistry;

    public:    
    Cache(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes);
    void reset();
    void reset(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes);
    void setCache(int cacheSizeInBytes, int cacheLineSizeInBytes, int sizeOfConnectedMemoryInBytes);
    std::bitset<8> locateByDirectMapping(std::bitset<13> address, int &cacheHit);
    std::bitset<8> locateByFullyAssociativeMapping(std::bitset<13> requestedAddress, int &cacheHit);
    std::bitset<8> locateBySetAssociativeMapping(int setSize, std::bitset<13> requestedAddress, int &cacheHit);
    void retrieveFromMemory(int blockIndex, BigPacket &packet);
    void connectToMemory(MemoryBlock *memoryConnection);
    int getLRUIndex();
    int getLRUIndex(int setSize, int setIndex);
    void updateLRUBlock(int lruValue);
    void updateLRUBlock(int lruValue, int setSize, int setIndex);
};

#endif