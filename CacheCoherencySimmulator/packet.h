#ifndef _PACKET_H
#define _PACKET_H

#include <bitset>

class Packet
{
    private:
    std::bitset<8> dataLSB;
    std::bitset<8> dataByte3;
    std::bitset<8> dataByte2;
    std::bitset<8> dataMSB;

    public:
    Packet(){
        this->dataLSB = std::bitset<8>(0);
        this->dataByte3 = std::bitset<8>(0);
        this->dataByte2 = std::bitset<8>(0);
        this->dataMSB = std::bitset<8>(0);
    };
    std::bitset<8> getDataLSB(){return this->dataLSB;};
    void setDataLSB(std::bitset<8> byte){this->dataLSB = byte;};
   
    std::bitset<8> getDataByte3(){return this->dataByte3;};
    void setDataByte3(std::bitset<8> byte){this->dataByte3 = byte;};

    std::bitset<8> getDataByte2(){return this->dataByte2;};
    void setDataByte2(std::bitset<8> byte){this->dataByte2 = byte;};

    std::bitset<8> getDataMSB(){return this->dataMSB;};
    void setDataMSB(std::bitset<8> byte){this->dataMSB = byte;};

};
#endif