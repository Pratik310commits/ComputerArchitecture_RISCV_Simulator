#pragma once
#include <string>
#include "cpu.h"
class SystemConfig
{
public:
	class MainMemoryConfg
	{
	public:
		std::string name;
		int sizeInBytes;
		int programSpaceInBytes;
		int bufferAfterStack;
		int programStartPosition;
	};
	class CPUConfig
	{
	public:
		std::string name;
		int programSizeInBytes;
		int stackMemorySizeInBytes;
	};
	MainMemoryConfg _mainMemoryConfg;
	CPUConfig _cpuConfg;
	std::string name;
	SystemConfig() {};
	SystemConfig(std::string name) { this->name = name; };
	void configure(std::string memoryName, int memSizeInBytes, int memProgSizeInBytes, int bufferAfterStack, int programStartPosition, std::string cpuName, int cpuProgramSizeInBytes, int stackMemorySizeInBytes);
};

