#include "instruction.h"
#include <algorithm>

Instruction::Instruction(void) { this->write = false; }

Instruction::Instruction(long clockCycle, std::string instructionName, bool writeNeeded, std::string instruction, std::string state, bool memoryRdOp)
{
    this->clockCyclesReq = clockCycle;
    //this->instruction = instruction;
    this->write = writeNeeded;
    this->instructionName = instructionName;
    this->state = state;
    this->memoryRdOp = memoryRdOp;
}

long Instruction::getReqClockCycle()
{
    return this->clockCyclesReq;
}

std::string Instruction::getType()
{
    return this->type;
}

bool Instruction::isWriteNeeded()
{
    return this->write;
}

void Instruction::setWriteOption(bool needed)
{
    this->write = needed;
}

bool Instruction::isMemoryReadNeeded()
{
    return this->memoryRdOp;
}

void Instruction::setMemoryReadOption(bool needed)
{
    this->memoryRdOp = needed;
}

std::string Instruction::getRawBinary() {
    return this->rawBinaryString;
}

void Instruction::setRawBinary(std::string value)
{
    this->rawBinaryString = value;
}

void Instruction::setReqClockCycle(long time)
{
    this->clockCyclesReq = time;
}

void Instruction::setType(std::string type)
{
    this->type = type;
}

std::string Instruction::getInstructionName()
{
    return this->instructionName;
}

void Instruction::setState(std::string state)
{
    this->state = state;
}

std::string Instruction::getState()
{
    return this->state;
}

void Instruction::setName(std::string name)
{
    this->instructionName = name;
}

void Instruction::setrs1(std::string value)
{
    this->rs1 = value;
}

std::string Instruction::getrs1()
{
    return this->rs1;
}

void Instruction::setrs2(std::string value) {
    this->rs2 = value;
}

std::string Instruction::getrs2()
{
    return this->rs2;
}

void Instruction::setrd(std::string value)
{
    this->rd = value;
}

std::string Instruction::getrd()
{
    return this->rd;
}

void Instruction::setimm(int value)
{
    this->imm = value;
}

int Instruction::getimm()
{
    return this->imm;
}

void Instruction::setFunc(std::string value)
{
    this->func = value;
}

std::string Instruction::getFunc()
{
    return this->func;
}

void Instruction::setOpcode(std::string opcode)
{
    this->opcode = opcode;
}

std::string Instruction::getOpcode()
{
    return this->opcode;
}

void Instruction::setOperation(std::string value)
{
    this->operation = value;
}

std::string Instruction::getOperation()
{
    return this->operation;
}

int Instruction::calculateInstructionLatency(Instruction nextInstruction)
{
    std::string currOp = this->operation;
    std::transform(currOp.begin(), currOp.end(), currOp.begin(), ::tolower);

    std::string nextOp = nextInstruction.operation;
    std::transform(nextOp.begin(), nextOp.end(), nextOp.begin(), ::tolower);

    if (currOp == "fp alu op")
    {
        if (nextOp == "fp alu op")
            return 3;
        else if (nextOp == "store double")
            return 2;
    }
    else if (currOp == "load double")
    {
        if (nextOp == "fp alu op")
            return 1;
        else if (nextOp == "store double")
            return 0;
    }
    else
        return 0;
}