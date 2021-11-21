#pragma once
#include <vector>
#include <string>
class Packet
{
private:
	std::vector<std::string> data;

public:
	Packet() {};
	Packet(int sizeOfBlockInBytes) { this->data = std::vector<std::string>(sizeOfBlockInBytes, ""); };
	void initialisePacket(int sizeOfBlockInBytes) { this->data = std::vector<std::string>(sizeOfBlockInBytes, ""); };
	std::vector<std::string> extractData() { return this->data; };
	void populateData(std::vector<std::string> data) { this->data = data; };
	std::string extractByteByIndex(int index) { return this->data[index]; };
	int getDataSizeInBytes() { return this->data.size(); };
};

