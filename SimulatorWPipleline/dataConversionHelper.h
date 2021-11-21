#pragma once
#include <string>
#include <vector>

class DataConversionHelper
{
public:
	static std::string getBitStreamFromFloat(float num, int numType);
	static float getFloatFromBitStream(std::string bitStream);
	static unsigned long getUnSignedLongIntegerFromBitStream(std::string bitStream, int len);
	static std::string addToBitStream(std::string bitStream, int addend);
	static std::string convertToBitStreamReverse(std::vector<std::string> packet);
	static int get32BitSignedIntegerFromBitStream(std::string binary);
	static std::string convertTo32BitStreamFromInt(int number);
	static int getSignedIntegerFromBitStream(std::string bitStream, int len);
	static std::string get32BitStreamFromFloat(float num, int numType);
	static float getFloatFrom32BitStream(std::string bitStream);
};

