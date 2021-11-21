#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "packet.h"
#include "instruction.h"
#include "mainMemory.h"
#include "event.h"
class Cpu
{
private:
    std::string name;
    long clockTick;
    std::vector<std::string> intRegisters;
    std::vector<std::string> floatRegisters;
    std::string pc;
    std::string sp;
    std::string programEndMarker;
    Instruction fetchedInstruction;
    Instruction decodedInstruction;
    Instruction executedALUInstruction;
    Instruction memReadInstruction;
    Instruction memWriteInstruction;
    std::string stackEndMarker;
    MainMemory* memoryConnection;
    int pipelineStall;
    std::vector<Event*>* systemMEQ;
    bool pcModified;
public:

    Cpu() {};

    void initialiseCPU(std::string name, std::string pc, std::string sp, std::string programEndMarker, std::string stackEndMarker, MainMemory* memoryConnection, std::vector<Event*>* systemMEQ);

    void checkForProgram();

    bool isEndOfInstructionReached();

    std::string getPC();

    std::string getName();

    int getPipelineStall();

    void setPipelineStall(int value);

    void setPC(std::string value);

    void setName(std::string name);

    void initialiseIntRegisters();

    void initialiseFloatRegisters();

    void setIntRegisterByIndex(int regIndex, std::string value);

    std::string getIntRegisterByIndex(int regIndex);

    void setFloatRegisterByIndex(int regIndex, std::string value);

    std::string getFloatRegisterByIndex(int regIndex);

    void updatePC();

    std::string getIntReg(int regIndex);

    int getIntRegIndex(std::string registerName);

    std::string getFloatReg(int regIndex);

    int getFloatRegIndex(std::string registerName);

    Packet getDataPacket(std::string packetBaseAddress);

    Packet getInstructionPacket(std::string packetBaseAddress);

    void fetchFromMemory(std::string packetBaseAddress, int dataSizeInBytes, Packet& packet);

    std::string convertTo32BitData(Packet packet);

    std::string extractOpcodeAsString(std::string rawBinary);

    void cleanInstructionObject(Instruction *toBeCleaned);

    void initialisePipelineInstructions();

    void fetchInstruction();

    void decodeInstruction();

    void executeALUInstruction();

    void executeWriteInstruction();

    void executeReadInstruction();

    int calculatePipelineStall(int reqClockCycles);

    void decodeITypeInstruction();

    void decodeSTypeInstruction();

    void decodeBTypeInstruction();

    void decodeRTypeInstruction();

    void decodeUJTypeInstruction();

    void decodeUTypeInstruction();

    void addi(std::string rd, std::string rs1, int imm);

    void add(std::string rd, std::string rs1, std::string rs2);

    void slli(std::string rd, std::string rs1, int imm);

    void auipc(std::string rd, int imm);

    void lw(std::string rd, int imm, std::string rs1);

    void sw(std::string rs2, int imm, std::string rs1);

    void blt(std::string rs1, std::string rs2, int offset);

    void jal(std::string rd, int imm);

    void jalr(std::string rd, std::string rs1, int imm);

    void lui(std::string rd, int imm);

    void flw(std::string rs1, std::string rd, int imm);

    void fsw(std::string rs1, std::string rs2, int imm);

    void fadds(std::string rs1, std::string rs2, std::string rd);

    void executePipelineEvent(long clockTick, Event* event);

    void createAndScheduleNextPipelineEvent(long arrivalTime, std::string by);

    void updatePipeline();

    void printInstructionStatement(Instruction instruction);
};

