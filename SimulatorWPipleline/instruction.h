#pragma once
#include <string>
class Instruction
{
private:
    std::string rawBinaryString;
	long clockCyclesReq;
	std::string type;
	std::string opcode;
    std::string instructionName;
    std::string rs1;
    std::string rs2;
    std::string rd;
    signed int imm;
    std::string func;
    bool write;
    std::string state;
    bool memoryRdOp;
    std::string operation;

public:

    Instruction(void);

    Instruction(long clockCycle, std::string instruction, bool write, std::string instructionName, std::string state, bool memoryRdOpNeeded);

    std::string getRawBinary();

    void setRawBinary(std::string value);

    long getReqClockCycle();

    bool isWriteNeeded();

    bool isMemoryReadNeeded();

    void setWriteOption(bool needed);

    void setMemoryReadOption(bool needed);

    void setReqClockCycle(long time);

    void setType(std::string format);

    void setName(std::string name);

    void setState(std::string state);

    std::string getState();

    void setrs1(std::string state);

    std::string getrs1();

    void setrs2(std::string state);

    std::string getrs2();

    void setrd(std::string state);

    std::string getrd();

    void setimm(int value);

    int getimm();

    std::string getType();

    std::string getInstructionName();

    std::string getFunc();

    void setFunc(std::string value);

    std::string getOpcode();

    void setOpcode(std::string opcode);

    void setOperation(std::string value);

    std::string getOperation();

    int calculateInstructionLatency(Instruction nextInstruction);

};

