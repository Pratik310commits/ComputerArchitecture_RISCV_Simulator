#include "system.h"
#include "dataConversionHelper.h"
#include <bitset>

void System::configureSystem(SystemConfig systemConfig)
{
    this->systemConfig = systemConfig;
}

void System::handleMEQEvent()
{
	if (this->MEQ.front()->getTypeID() == "pipeline event")
	{
        std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t--------- Clock tick: " << this->clockTick << " ---------" << std::endl;
        while (this->MEQ.front()->getArrivalTime() == this->clockTick)
        {
            if (this->cpu->getName() == this->MEQ.front()->getAssignedCPUID())
            {
                this->cpu->executePipelineEvent(this->clockTick, this->MEQ.front());
                this->MEQ.erase(this->MEQ.begin());
            }
        }
        ++this->clockTick;
		
	}
}

void System::run()
{
    this->cpu = new Cpu;
    this->ram = new MainMemory;
	//this->systemConfig = SystemConfig();
    this->initialiseSystem();
    this->cpu->checkForProgram();
    this->clockTick = 1;
    while (this->MEQ.size() > 0)
    {
        this->handleMEQEvent();
    }
}

void System::initialiseSystem()
{
	this->cpu->setName(this->systemConfig._cpuConfg.name);
	this->ram->initialiseMemoryBlock(this->systemConfig._mainMemoryConfg.sizeInBytes);
	this->ram->name = this->systemConfig._mainMemoryConfg.name;

	//initialiseCPU(std::string name, std::string pc, std::string sp, std::string programEndMarker, std::string stackEndMarker, MainMemory * memoryConnection)

	std::string pc = DataConversionHelper::convertTo32BitStreamFromInt(this->systemConfig._mainMemoryConfg.programStartPosition);

	////The stack pointer always points to the item that is currently at the top of the stack.
	std::string sp = DataConversionHelper::convertTo32BitStreamFromInt(this->systemConfig._mainMemoryConfg.programStartPosition + this->systemConfig._mainMemoryConfg.programSpaceInBytes + this->systemConfig._cpuConfg.stackMemorySizeInBytes - 1);
	std::string programEndMarker = DataConversionHelper::convertTo32BitStreamFromInt(this->systemConfig._mainMemoryConfg.programStartPosition + this->systemConfig._cpuConfg.programSizeInBytes - 1);
	std::string stackEndMarker = DataConversionHelper::convertTo32BitStreamFromInt(this->systemConfig._mainMemoryConfg.programStartPosition + this->systemConfig._mainMemoryConfg.programSpaceInBytes);

	this->cpu->initialiseCPU(this->systemConfig._cpuConfg.name, pc, sp, programEndMarker, stackEndMarker, this->ram, &(this->MEQ));


    ////initialise RAM
    std::vector<std::string> program;
    this->initProgramCodeForCPU(&program);
    int firstIndex = this->systemConfig._mainMemoryConfg.programStartPosition;
    for (int i = 0; i < this->systemConfig._cpuConfg.programSizeInBytes; ++i)
    {
        this->ram->getMemorySpace()->at(firstIndex + i) =  program.at(i);
    }

    //// initialise data
    std::cout << DataConversionHelper::getBitStreamFromFloat(1, 32) << std::endl;
    std::cout << DataConversionHelper::getFloatFromBitStream(DataConversionHelper::getBitStreamFromFloat(1, 32)) << std::endl;

    for (int i = 0; i < 1024; ++i)
    {
        std::string dataStr = DataConversionHelper::getBitStreamFromFloat(i + 1, 32);
        this->ram->getMemorySpace()->at(1024 + i + 0) = dataStr.substr(24, 8);
        this->ram->getMemorySpace()->at(1024 + i + 1) = dataStr.substr(16, 8);
        this->ram->getMemorySpace()->at(1024 + i + 2) = dataStr.substr(8, 8);
        this->ram->getMemorySpace()->at(1024 + i + 3) = dataStr.substr(0, 8);

        dataStr = DataConversionHelper::getBitStreamFromFloat(i + 1, 32);
        this->ram->getMemorySpace()->at(2048 + i + 0) = dataStr.substr(24, 8);
        this->ram->getMemorySpace()->at(2048 + i + 1) = dataStr.substr(16, 8);
        this->ram->getMemorySpace()->at(2048 + i + 2) = dataStr.substr(8, 8);
        this->ram->getMemorySpace()->at(2048 + i + 3) = dataStr.substr(0, 8);
        i = i + 4;
    }
    
    std::cout << this->ram->getMemorySpace()->at(1027) + this->ram->getMemorySpace()->at(1026) + this->ram->getMemorySpace()->at(1025) + this->ram->getMemorySpace()->at(1024) << std::endl;

    std::cout << "Data @ 1024: " << DataConversionHelper::getFloatFromBitStream(this->ram->getMemorySpace()->at(1027) + this->ram->getMemorySpace()->at(1026) + this->ram->getMemorySpace()->at(1025) + this->ram->getMemorySpace()->at(1024)) << std::endl;

    std::cout << "Data @ 2048: " << DataConversionHelper::getFloatFromBitStream(this->ram->getMemorySpace()->at(2051) + this->ram->getMemorySpace()->at(2050) + this->ram->getMemorySpace()->at(2049) + this->ram->getMemorySpace()->at(2048)) << std::endl;

}

void System::initProgramCodeForCPU(std::vector<std::string>* program)
{
    ////addi sp, sp, -16
    program->push_back("00010011");
    program->push_back("00000001");
    program->push_back("00000001");
    program->push_back("11111111");

    ////sw ra, 12(sp)
    program->push_back("00100011");
    program->push_back("00100110");
    program->push_back("00010001");
    program->push_back("00000000");

    ////sw s0, 8(sp)
    program->push_back("00100011");
    program->push_back("00100100");
    program->push_back("10000001");
    program->push_back("00000000");

    ////addi s0, sp, 16
    program->push_back("00010011");
    program->push_back("00000100");
    program->push_back("00000001");
    program->push_back("00000001");

    ////mv a0, zero
    ////ADDI a0, x0, 0
    program->push_back("00010011");
    program->push_back("00000101");
    program->push_back("00000000");
    program->push_back("00000000");

    ////sw a0, -12(s0)
    program->push_back("00100011");
    program->push_back("00101010");
    program->push_back("10100100");
    program->push_back("11111110");

    ////sw a0, -16(s0)
    program->push_back("00100011");
    program->push_back("00101000");
    program->push_back("10100100");
    program->push_back("11111110");

    ////j .LBB0_1
    ////JAL x0, 32
    program->push_back("01101111");
    program->push_back("00000000");
    program->push_back("00000000");
    program->push_back("00000010");

    ////lw a0, -16(s0)
    program->push_back("00000011");
    program->push_back("00100101");
    program->push_back("00000100");
    program->push_back("11111111");

    ////addi a1, zero, 255
    program->push_back("10010011");
    program->push_back("00000101");
    program->push_back("11110000");
    program->push_back("00001111");

    ////blt a1, a0, .LBB0_4
    program->push_back("01100011");
    program->push_back("11000000");
    program->push_back("10100101");
    program->push_back("00010000");

    ////j .LBB0_2
    ////JAL x0, 48
    ////000000000000001100000 incorrect
    ////000000000000011000000
    ////000000000000000110000 correct
    program->push_back("01101111");
    program->push_back("00000000");
    program->push_back("00000000");
    program->push_back("00000011");

    ////lui a0, %hi(1024)
    program->push_back("00110111");
    program->push_back("00000101");
    program->push_back("00000000");
    program->push_back("00000000");

    ////lw a0, %lo(1024)(a0)
    ////addi a0, a0, %lo(1024) --  new
    program->push_back("00010011");
    program->push_back("00000101");
    program->push_back("00000101");
    program->push_back("01000000");

    ////lw a1, -16(s0)
    program->push_back("10000011");
    program->push_back("00100101");
    program->push_back("00000100");
    program->push_back("11111111");

    ////slli a1, a1, 2
    program->push_back("10010011");
    program->push_back("10010101");
    program->push_back("00100101");
    program->push_back("00000000");

    ////add a0, a0, a1
    program->push_back("00110011");
    program->push_back("00000101");
    program->push_back("10110101");
    program->push_back("00000000");


    ////flw ft0, 0(a0)
    program->push_back("00000111");
    program->push_back("00100000");
    program->push_back("00000101");
    program->push_back("00000000");


    ////lui a0, %hi(2048)
    program->push_back("00110111");
    program->push_back("00000101");
    program->push_back("10000000");
    program->push_back("00000000");


    ////lw a0, %lo(2048)(a0)
    ////addi a0, a0, %lo(2048) -- new
    ////interpreted wrongly as ADDI a0, a0, -2048

    program->push_back("00010011");
    program->push_back("00000101");
    program->push_back("00000101");
    program->push_back("10000000");

    ////add a0, a0, a1
    program->push_back("00110011");
    program->push_back("00000101");
    program->push_back("10110101");
    program->push_back("00000000");

    ////flw ft1, 0(a0)
    ////00000000 00000101 00100000 10000111
    program->push_back("10000111");
    program->push_back("00100000");
    program->push_back("00000101");
    program->push_back("00000000");


    ////fadd.s ft0, ft0, ft1
    ////00000000 00010000 01110000 01010011

    program->push_back("01010011");
    program->push_back("01110000");
    program->push_back("00010000");
    program->push_back("00000000");

    ////lui a0, %hi(3072)
    program->push_back("00000011");
    program->push_back("00100101");
    program->push_back("00000101");
    program->push_back("11000000");


    ////lw a0, %lo(3072)(a0)
    program->push_back("00110011");
    program->push_back("00000101");
    program->push_back("10110101");
    program->push_back("00000000");

    ////add a0, a0, a1
    program->push_back("00100111");
    program->push_back("00100000");
    program->push_back("01100101");
    program->push_back("00000001");

    ////fsw ft0, 0(a0)
    program->push_back("01101111");
    program->push_back("00000000");
    program->push_back("00000000");
    program->push_back("00001110");

    ////j .LBB0_3
    ////JAL x0, 112
    program->push_back("00000011");
    program->push_back("00100101");
    program->push_back("00000100");
    program->push_back("11111111");

    ////
    program->push_back("00010011");
    program->push_back("00000101");
    program->push_back("00010101");
    program->push_back("00000000");


    ////
    program->push_back("00010011");
    program->push_back("00000101");
    program->push_back("00010101");
    program->push_back("00000000");

    ////
    program->push_back("00100011");
    program->push_back("00101000");
    program->push_back("10100100");
    program->push_back("11111110");

    ////
    program->push_back("01101111");
    program->push_back("00000000");
    program->push_back("00000000");
    program->push_back("00000100");

    ////
    program->push_back("00000011");
    program->push_back("00100101");
    program->push_back("01000100");
    program->push_back("11111111");

    ////
    program->push_back("00000011");
    program->push_back("00100100");
    program->push_back("10000001");
    program->push_back("00000000");

    ////
    program->push_back("00000011");
    program->push_back("10100100");
    program->push_back("11000000");
    program->push_back("00000000");

    ////
    program->push_back("00000011");
    program->push_back("00000001");
    program->push_back("00000001");
    program->push_back("00000001");

    ////
    program->push_back("01100111");
    program->push_back("10000000");
    program->push_back("00000000");
    program->push_back("00000000");

}

