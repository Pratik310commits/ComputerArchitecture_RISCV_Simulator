#ifndef _BIG_PACKET_H
#define _BIG_PACKET_H

#include <bitset>

class BigPacket
{
    private:
    std::vector<std::bitset<8>> block;

    public:
    BigPacket(){};
    BigPacket(int sizeOfBlockInBytes){this->block = std::vector<std::bitset<8>> (sizeOfBlockInBytes, 0);};
    std::vector<std::bitset<8>> extractBlock(){return this->block;};
    void populateBlockData(std::vector<std::bitset<8>> data){this->block = data;};
    std::bitset<8> extractByteByIndex(int index){return this->block[index];};
};
#endif