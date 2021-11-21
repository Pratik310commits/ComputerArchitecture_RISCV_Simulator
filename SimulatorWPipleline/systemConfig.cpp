#include "systemConfig.h"
#include "dataConversionHelper.h"



void SystemConfig::configure(std::string memoryName, int memSizeInBytes, int memProgSizeInBytes, int bufferAfterStack, int programStartPosition, std::string cpuName, int cpuProgramSizeInBytes, int stackMemorySizeInBytes)
{
	this->_mainMemoryConfg.name = memoryName;
	this->_mainMemoryConfg.sizeInBytes = memSizeInBytes;
	this->_mainMemoryConfg.programSpaceInBytes = memProgSizeInBytes;
	this->_mainMemoryConfg.bufferAfterStack = bufferAfterStack;
	this->_mainMemoryConfg.programStartPosition = programStartPosition;
	this->_cpuConfg.name = cpuName;
	this->_cpuConfg.programSizeInBytes = cpuProgramSizeInBytes;
	this->_cpuConfg.stackMemorySizeInBytes = stackMemorySizeInBytes;
}