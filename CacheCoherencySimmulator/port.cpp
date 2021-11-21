#include "port.h"

Port::Port()
{
    //
}

void Port::connectToMemory(MemoryBlock *memoryConnection)
{
    this->memoryConnection = memoryConnection;
}

void Port::populateData(MemoryBlock *memoryConnection, std::bitset<8> dataAddress, Packet dataPacket)
{
    std::bitset<8> *tempPtr = &dataAddress;
    memoryConnection->setDataByAddress(tempPtr, dataPacket.getDataLSB());
    memoryConnection->setDataByAddress(tempPtr + 1, dataPacket.getDataByte3());
    memoryConnection->setDataByAddress(tempPtr + 2, dataPacket.getDataByte2());
    memoryConnection->setDataByAddress(tempPtr + 3, dataPacket.getDataMSB());
}

void Port::retrieveData(MemoryBlock *memoryConnection, std::bitset<8> &dataAddress, Packet &dataPacket)
{
    std::bitset<8> *tempPtr = &dataAddress;
    dataPacket.setDataLSB(memoryConnection->getDataByAddress(tempPtr));
    dataPacket.setDataByte3(memoryConnection->getDataByAddress(tempPtr + 1));
    dataPacket.setDataByte2(memoryConnection->getDataByAddress(tempPtr + 2));
    dataPacket.setDataMSB(memoryConnection->getDataByAddress(tempPtr + 3));
}

bool Port::isPortConnected()
{
    return this->isConnected;
}

void Port::setState(bool state)
{
    this->isConnected = state;
}

void Port::initialisePort()
{
    this->dataAddress = std::bitset<8>{}.reset();
    // this->startAddressPtr = std::bitset<8>{}.reset();
    // this->endAddressPtr = std::bitset<8>{}.reset();
}

void Port::setStartAddressPtr(std::bitset<8> &startAddressPtr)
{
    this->startAddressPtr = &startAddressPtr;
}

void Port::setEndAddressPtr(std::bitset<8> &endAddressPtr)
{
    this->endAddressPtr = &endAddressPtr;
}

std::bitset<8>* Port::getEndAddressPtr()
{
    return this->endAddressPtr;
}

void Port::getDataByBlock(int blockSizeInBytes, int blockIndex, BigPacket &packet)
{
    this->memoryConnection->getDataByBlock(blockSizeInBytes, blockIndex, packet);
}