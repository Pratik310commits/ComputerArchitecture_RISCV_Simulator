#ifndef _PORT_H
#define _PORT_H

#include <bitset>
#include "packet.h"
#include "memoryBlock.h"
#include "bigPacket.h"

class Port
{
    private:
    std::bitset<8> dataAddress;
    std::bitset<8> *startAddressPtr;
    std::bitset<8> *endAddressPtr;
    MemoryBlock *memoryConnection;
    bool isConnected;
    public:
    Port();
    void populateData(MemoryBlock *memoryConnection, std::bitset<8> dataAddress, Packet dataPacket);
    void retrieveData(MemoryBlock *memoryConnection, std::bitset<8> &dataAddress, Packet &dataPacket);
    bool isPortConnected();
    void setState(bool state);
    void initialisePort();
    void setStartAddressPtr(std::bitset<8> &startAddressPtr);
    void setEndAddressPtr(std::bitset<8> &endAddressPtr);
    std::bitset<8>* getEndAddressPtr();
    void getDataByBlock(int blockSizeInBytes, int blockIndex, BigPacket &packet);
    void connectToMemory(MemoryBlock *memoryConnection);
};
#endif