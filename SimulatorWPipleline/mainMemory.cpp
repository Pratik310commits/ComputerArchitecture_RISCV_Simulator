#include "mainMemory.h"

void MainMemory::getDataByAddress(std::string address, int dataSizeInBytes, Packet& packet)
{
    long addressIndex = (long)std::stoul(address, nullptr, 2);
    //int dataSizeInBytes = packet.getDataSizeInBytes(); //// this is not the responsibility of this method
    std::vector<std::string> data; //intialise, otherwise it is NULL

    for (int itr = 0; itr < dataSizeInBytes; ++itr)
    {
        data.push_back(this->getMemorySpace()->at(addressIndex + itr));
    }
    packet.populateData(data);
}

void MainMemory::setDataByAddress(std::string address, int dataSizeInBytes, Packet packet)
{
    long addressIndex = (long)std::stoul(address, nullptr, 2);
    //int dataSizeInBytes = packet.getDataSizeInBytes(); //// this is not the responsibility of this method

    for (int itr = 0; itr < dataSizeInBytes; ++itr)
    {
        this->getMemorySpace()->at(addressIndex + itr) = packet.extractByteByIndex(itr);
    }
}

void MainMemory::getDataByBlock(int blockSizeInBytes, int blockIndex, Packet& packet)
{
    //divide block size by 8, this will give number of bytes in single block = sb
    //bi * sb, this is the index
    //use the index to retrieve total sb bytes
    int bytesInSingleBlock = blockSizeInBytes / 8;
    int memoryIndex = blockIndex * bytesInSingleBlock;

    /*if (packet == NULL)
    {
        packet.initialisePacket(blockSizeInBytes);
    }*/
    std::vector<std::string> data;

    for (int itr = 0; itr < bytesInSingleBlock; ++itr)
    {
        data.push_back(this->getMemorySpace()->at(memoryIndex + itr));
    }
    packet.populateData(data);
}

void MainMemory::setDataByBlock(int blockSizeInBytes, int blockIndex, Packet packet)
{
    int bytesInSingleBlock = blockSizeInBytes / 8;
    int memoryIndex = blockIndex * bytesInSingleBlock;

    for (int itr = 0; itr < bytesInSingleBlock; ++itr)
    {
        this->getMemorySpace()->at(memoryIndex + itr) = packet.extractByteByIndex(itr);
    }
}