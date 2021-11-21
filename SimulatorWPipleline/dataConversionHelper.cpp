#include "dataConversionHelper.h"
#include <bitset>


std::string DataConversionHelper::getBitStreamFromFloat(float num, int len)
{
    int* y, x;
    std::string bitStream = "";
    y = reinterpret_cast<int*>(&num);
    x = *y;
    for (short i = 0; i < len; ++i) {
        bitStream += std::to_string(x % 2);
        x /= 2;
    }
    return bitStream;
}

unsigned long DataConversionHelper::getUnSignedLongIntegerFromBitStream(std::string bitStream, int len)
{
    int limit = len - bitStream.length();
    std::string extendedBinary = "";
    for (int i = 0; i < limit; ++i)
    {
        extendedBinary += bitStream[0];
    }
    extendedBinary += bitStream;
    unsigned long ul = std::stoul(extendedBinary, nullptr, 2);
    return ul;
}

float DataConversionHelper::getFloatFromBitStream(std::string bitStream) {
    int* x, y;
    y = std::stoul(bitStream, nullptr, 2);
    x = &y;
    float* num = reinterpret_cast<float*>(x);
    return *num;
}

std::string DataConversionHelper::addToBitStream(std::string bitStream, int addend)
{
    long bitData = (long)std::stoul(bitStream, nullptr, 2);
    return std::bitset<32>(bitData + addend).to_string();
}

std::string DataConversionHelper::convertToBitStreamReverse(std::vector<std::string> packet)
{
    //
    std::string tempBitDataString = "";
    for (int itr = packet.size() - 1; itr >= 0; --itr)
    {
        tempBitDataString += packet.at(itr);
    }
    //tempBitDataString = packet.getDataMSB().to_string() + packet.getDataByte2().to_string() + packet.getDataByte3().to_string() + packet.getDataLSB().to_string();
    return tempBitDataString;
}

int DataConversionHelper::get32BitSignedIntegerFromBitStream(std::string binary)
{
    std::bitset<32> b(binary);
    int32_t x32 = (int32_t)b.to_ulong();
    return x32;
}

std::string DataConversionHelper::convertTo32BitStreamFromInt(int number)
{
    return std::bitset<32>(number).to_string();
}

int DataConversionHelper::getSignedIntegerFromBitStream(std::string bitStream, int len)
{
    int limit = len - bitStream.length();
    std::string padingBit = ((bitStream[0] == bitStream[1]) && bitStream[0] == '1') ? "1" : "0";
    std::string extendedBinary = "";
    for (int i = 0; i < limit; ++i)
    {
        extendedBinary += padingBit;
    }
    extendedBinary += bitStream;
    unsigned long ul = std::stoul(extendedBinary, nullptr, 2);
    return ul;
}

std::string get32BitStreamFromFloat(float num, int numType)
{
    int mantissa = 23;
    unsigned int exponent = 8;
}

float getFloatFrom32BitStream(std::string bitStream)
{

}

