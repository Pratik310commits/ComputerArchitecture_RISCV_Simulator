#include "cpu.h"
#include "dataConversionHelper.h"
#include <bitset>
#include <algorithm>
#include <typeinfo>
#include "fetchInstructionEvent.h"
#include "decodeInstructionEvent.h"
#include "executeALUInstructionEvent.h"
#include "memoryReadInstructionEvent.h"
#include "memoryWriteInstructionEvent.h"



void Cpu::initialiseCPU(std::string name, std::string pc, std::string sp, std::string programEndMarker, std::string stackEndMarker, MainMemory * memoryConnection, std::vector<Event*>* systemMEQ)
{
    this->name = name;
    this->pc = pc;
    this->sp = sp;
    
    this->pcModified = false;
    this->programEndMarker = programEndMarker;
    this->stackEndMarker = stackEndMarker;
    this->memoryConnection = memoryConnection;
    this->systemMEQ = systemMEQ;
    this->initialiseIntRegisters();
    this->setIntRegisterByIndex(this->getIntRegIndex("sp"), sp);
    this->initialiseFloatRegisters();
    this->initialisePipelineInstructions();
}

void Cpu::checkForProgram()
{
    if (!this->isEndOfInstructionReached())
    {
        this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "fetch");
    }
    else
    {
        this->cleanInstructionObject(&this->fetchedInstruction);
    }
}

std::string Cpu::getName()
{
    return this->name;
}

void Cpu::setName(std::string name)
{
    this->name = name;
}

int Cpu::getPipelineStall()
{
    return this->pipelineStall;
}

void Cpu::setPipelineStall(int value)
{
    this->pipelineStall = value;
}

bool Cpu::isEndOfInstructionReached()
{
	int pcIndex = (int)std::stoul(this->pc, nullptr, 2);
	int programEndMarker = (int)std::stoul(this->programEndMarker, nullptr, 2);
	return pcIndex == programEndMarker - 3;
}

std::string Cpu::getPC()
{
	return this->pc;
}

void Cpu::setPC(std::string value)
{
	this->pc = value;
}

void Cpu::initialiseIntRegisters()
{
	this->intRegisters = std::vector<std::string>(32, "00000000000000000000000000000000");
}

void Cpu::Cpu::initialiseFloatRegisters()
{
	this->floatRegisters = std::vector<std::string>(32, "00000000000000000000000000000000");
}

void Cpu::setIntRegisterByIndex(int regIndex, std::string value)
{
	this->intRegisters[regIndex] = value;
}

std::string Cpu::getIntRegisterByIndex(int regIndex)
{
	return this->intRegisters[regIndex];
}

void Cpu::setFloatRegisterByIndex(int regIndex, std::string value)
{
	this->floatRegisters[regIndex] = value;
}

std::string Cpu::getFloatRegisterByIndex(int regIndex)
{
	return this->floatRegisters[regIndex];
}

void Cpu::updatePC()
{
	//long pcIndex = (long)std::stoul(this->pc, nullptr, 2);

	this->setPC(DataConversionHelper::addToBitStream(this->pc, 4));
    std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tpc: " << DataConversionHelper::get32BitSignedIntegerFromBitStream(this->pc) << std::endl;
}

std::string Cpu::getIntReg(int regIndex)
{
    switch (regIndex) {
    case 0:
        return "x0";
    case 1:
        return "ra";
    case 2:
        return "sp";
    case 3:
        return "gp";
    case 4:
        return "tp";
    case 5:
        return "t0";
    case 6:
        return "t1";
    case 7:
        return "t2";
    case 8:
        return "s0";
    case 9:
        return "s1";
    case 10:
        return "a0";
    case 11:
        return "a1";
    case 12:
        return "a2";
    case 13:
        return "a3";
    case 14:
        return "a4";
    case 15:
        return "a5";
    case 16:
        return "a6";
    case 17:
        return "a7";
    case 18:
        return "s2";
    case 19:
        return "s3";
    case 20:
        return "s4";
    case 21:
        return "s5";
    case 22:
        return "s6";
    case 23:
        return "s7";
    case 24:
        return "s8";
    case 25:
        return "s9";
    case 26:
        return "s10";
    case 27:
        return "s11";
    case 28:
        return "t3";
    case 29:
        return "t4";
    case 30:
        return "t5";
    case 31:
        return "t6";
    default:
        return "";
    }
}

int Cpu::getIntRegIndex(std::string registerName)
{
    std::transform(registerName.begin(), registerName.end(), registerName.begin(), ::tolower);

    if (registerName == "zero" || registerName == "x0")
        return 0;
    else if (registerName == "ra" || registerName == "x1")
        return 1;
    else if (registerName == "sp" || registerName == "x2")
        return 2;
    else if (registerName == "gp" || registerName == "x3")
        return 3;
    else if (registerName == "tp" || registerName == "x4")
        return 4;
    else if (registerName == "t0" || registerName == "x5")
        return 5;
    else if (registerName == "t1" || registerName == "x6")
        return 6;
    else if (registerName == "t2" || registerName == "x7")
        return 7;
    else if (registerName == "s0" || registerName == "fp" || registerName == "x8")
        return 8;
    else if (registerName == "s1" || registerName == "x9")
        return 9;
    else if (registerName == "a0" || registerName == "x10")
        return 10;
    else if (registerName == "a1" || registerName == "x11")
        return 11;
    else if (registerName == "a2" || registerName == "x12")
        return 12;
    else if (registerName == "a3" || registerName == "x13")
        return 13;
    else if (registerName == "a4" || registerName == "x14")
        return 14;
    else if (registerName == "a5" || registerName == "x15")
        return 15;
    else if (registerName == "a6" || registerName == "x16")
        return 16;
    else if (registerName == "a7" || registerName == "x17")
        return 17;
    else if (registerName == "s2" || registerName == "x18")
        return 18;
    else if (registerName == "s3" || registerName == "x19")
        return 19;
    else if (registerName == "s4" || registerName == "x20")
        return 20;
    else if (registerName == "s5" || registerName == "x21")
        return 21;
    else if (registerName == "s6" || registerName == "x22")
        return 22;
    else if (registerName == "s7" || registerName == "x23")
        return 23;
    else if (registerName == "s8" || registerName == "x24")
        return 24;
    else if (registerName == "s9" || registerName == "x25")
        return 25;
    else if (registerName == "s10" || registerName == "x26")
        return 26;
    else if (registerName == "s11" || registerName == "x27")
        return 27;
    else if (registerName == "t3" || registerName == "x28")
        return 28;
    else if (registerName == "t4" || registerName == "x29")
        return 29;
    else if (registerName == "t5" || registerName == "x30")
        return 30;
    else if (registerName == "t6" || registerName == "x31")
        return 31;
}

std::string Cpu::getFloatReg(int regIndex)
{
    switch (regIndex) {
    case 0:
        return "f0";
    case 1:
        return "f1";
    case 2:
        return "f2";
    case 3:
        return "f3";
    case 4:
        return "f4";
    case 5:
        return "f5";
    case 6:
        return "f6";
    case 7:
        return "f7";
    case 8:
        return "f8";
    case 9:
        return "f9";
    case 10:
        return "f10";
    case 11:
        return "f11";
    case 12:
        return "f12";
    case 13:
        return "f13";
    case 14:
        return "f14";
    case 15:
        return "f15";
    case 16:
        return "f16";
    case 17:
        return "f17";
    case 18:
        return "f18";
    case 19:
        return "f19";
    case 20:
        return "f20";
    case 21:
        return "f21";
    case 22:
        return "f22";
    case 23:
        return "f23";
    case 24:
        return "f24";
    case 25:
        return "f25";
    case 26:
        return "f26";
    case 27:
        return "f27";
    case 28:
        return "f28";
    case 29:
        return "f29";
    case 30:
        return "f30";
    case 31:
        return "f31";
    }
}

int Cpu::getFloatRegIndex(std::string registerName)
{
    std::transform(registerName.begin(), registerName.end(), registerName.begin(), ::tolower);
    if (registerName == "f0" || registerName == "ft0")
        return 0;
    else if (registerName == "f1" || registerName == "ft1")
        return 1;
    else if (registerName == "f2" || registerName == "ft2")
        return 2;
    else if (registerName == "f3" || registerName == "ft3")
        return 3;
    else if (registerName == "f4" || registerName == "ft4")
        return 4;
    else if (registerName == "f5" || registerName == "ft5")
        return 5;
    else if (registerName == "f6" || registerName == "ft6")
        return 6;
    else if (registerName == "f7" || registerName == "ft7")
        return 7;
    else if (registerName == "f8" || registerName == "fs0")
        return 8;
    else if (registerName == "f9" || registerName == "fs1")
        return 9;
    else if (registerName == "f10" || registerName == "fa0")
        return 10;
    else if (registerName == "f11" || registerName == "fa1")
        return 11;
    else if (registerName == "f12" || registerName == "fa2")
        return 12;
    else if (registerName == "f13" || registerName == "fa3")
        return 13;
    else if (registerName == "f14" || registerName == "fa4")
        return 14;
    else if (registerName == "f15" || registerName == "fa5")
        return 15;
    else if (registerName == "f16" || registerName == "fa6")
        return 16;
    else if (registerName == "f17" || registerName == "fa7")
        return 17;
    else if (registerName == "f18" || registerName == "fs2")
        return 18;
    else if (registerName == "f19" || registerName == "fs3")
        return 19;
    else if (registerName == "f20" || registerName == "fs4")
        return 20;
    else if (registerName == "f21" || registerName == "fs5")
        return 21;
    else if (registerName == "f22" || registerName == "fs6")
        return 22;
    else if (registerName == "f23" || registerName == "fs7")
        return 23;
    else if (registerName == "f24" || registerName == "fs8")
        return 24;
    else if (registerName == "f25" || registerName == "fs9")
        return 25;
    else if (registerName == "f26" || registerName == "fs10")
        return 26;
    else if (registerName == "f27" || registerName == "fs11")
        return 27;
    else if (registerName == "f28" || registerName == "ft8")
        return 28;
    else if (registerName == "f29" || registerName == "ft9")
        return 29;
    else if (registerName == "f30" || registerName == "ft10")
        return 30;
    else if (registerName == "f31" || registerName == "ft11")
        return 31;
    return 0;
}

void Cpu::fetchInstruction()
{
    
    Packet instructionPacket = this->getInstructionPacket(this->pc);
    std::string rawBinary = DataConversionHelper::convertToBitStreamReverse(instructionPacket.extractData());
    this->fetchedInstruction.setRawBinary(rawBinary);
    this->fetchedInstruction.setState("fetched");
    this->fetchedInstruction.setWriteOption(false);
    this->fetchedInstruction.setOperation("");
    std::cout << "Fetch   | Instruction Opcode: " << this->extractOpcodeAsString(rawBinary) << std::endl;
    //std::string log = "Fetch   | Instruction: ";
    //this->eventLog.push_back(log.append(this->fetchUnit->getInstruction()));        

    
}

void Cpu::decodeInstruction()
{
    if (this->decodedInstruction.getState() == "fetched")
    {
        std::string opcode = this->extractOpcodeAsString(this->decodedInstruction.getRawBinary());
        this->decodedInstruction.setOpcode(opcode);
        /*
        decode the instruction Type from opcode
        */
        if (opcode == "0000011" || opcode == "0010011" || opcode == "1100111" || opcode == "0011011" || opcode == "0000111")
        {
            //lw || addi || jalr || slli(same as addi) || ADDIW || flw
            this->decodedInstruction.setType("IType");
            this->decodeITypeInstruction();
        }
        else if (opcode == "0110011" || opcode == "1010011")
        {
            //add || xor || mul
            this->decodedInstruction.setType("RType");
            this->decodeRTypeInstruction();
        }
        else if (opcode == "0100011" || opcode == "0100111") {
            //sw || sb || sh || sd ||fsw || fsd
            this->decodedInstruction.setType("SType");
            this->decodeSTypeInstruction();
        }
        else if (opcode == "1100011") {
            //BEQ || BLT || BLTU || BGEU || BNE || BGE
            this->decodedInstruction.setType("BType");
            this->decodeBTypeInstruction();
        }
        else if (opcode == "0110111" || opcode == "0010111") {
            //lui || auipc
            this->decodedInstruction.setType("UType");
            this->decodeUTypeInstruction();
        }
        else if (opcode == "1101111") {
            //jal
            this->decodedInstruction.setType("UJType");
            this->decodeUJTypeInstruction();
        }
        this->decodedInstruction.setState("decoded");
        std::cout << "Decode   | Instruction name: " << this->decodedInstruction.getInstructionName() << std::endl;
    }
    
}

void Cpu::decodeITypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string imm = "";
    std::string rs1 = "";
    std::string func = "";
    std::string rd = "";

    imm = rawBinary.substr(0, 12);
    rs1 = rawBinary.substr(12, 5);
    func = rawBinary.substr(17, 3);
    rd = rawBinary.substr(20, 5);

    this->decodedInstruction.setrs2("NA");

    int reg = std::stoul(rs1, nullptr, 2);
    this->decodedInstruction.setrs1(this->getIntReg(reg));
    reg = std::stoul(rd, nullptr, 2);
    this->decodedInstruction.setrd(this->getIntReg(reg));

    this->decodedInstruction.setFunc(func);
    
    //unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(imm, 32);
    //int tempp = DataConversionHelper::getSignedIntegerFromBitStream(imm, 32);
    this->decodedInstruction.setimm(DataConversionHelper::getSignedIntegerFromBitStream(imm, 32));
    std::string opcode = this->decodedInstruction.getOpcode();
    if (opcode == "0000011")
    {
        if (func == "000")
            this->decodedInstruction.setName("LB");
        else if (func == "110")
            this->decodedInstruction.setName("LWU");
        else if (func == "011")
        {
            this->decodedInstruction.setName("LD");
            this->decodedInstruction.setOperation("Load double");
        }
        else if (func == "001")
            this->decodedInstruction.setName("LH");
        else if (func == "010")
            this->decodedInstruction.setName("LW");
        else if (func == "100")
            this->decodedInstruction.setName("LBU");
        else if (func == "101")
            this->decodedInstruction.setName("LHU");

        this->decodedInstruction.setMemoryReadOption(true);
        //000 0000011 LB
        //110 0000011 LWU
        //011 0000011 LD
        //001 0000011 LH
        //010 0000011 lw
        //100 0000011 lbu
        //101 0000011 lhu
    }
    else if (opcode == "0011011")
    {
        this->decodedInstruction.setName("ADDIW");
        //000 0011011 ADDIW
    }
    else if (opcode == "0000111")
    {
        reg = std::stoul(rs1, nullptr, 2);
        this->decodedInstruction.setrs1(this->getIntReg(reg));
        reg = std::stoul(rd, nullptr, 2);
        this->decodedInstruction.setrd(this->getFloatReg(reg));

        if (func == "010")
            this->decodedInstruction.setName("FLW");
        else if (func == "011")
        {
            this->decodedInstruction.setName("FLD");
            this->decodedInstruction.setOperation("Load double");
        }
            
        this->decodedInstruction.setMemoryReadOption(true);
        //010 0000111 FLW
        //011 0000111 FLD
    }
    else if (opcode == "1100111")
    {
        this->decodedInstruction.setName("JALR");
        //000 1100111 JALR
    }
    else if (opcode == "0010011")
    {
        //000 0010011 addi
        //010 0010011 slti
        //011 0010011 sltiu
        //100 0010011 xori
        //110 0010011 ori
        //111 0010011 andi
        if (func == "000")
            this->decodedInstruction.setName("ADDI");
        else if (func == "010")
            this->decodedInstruction.setName("SLTI");
        else if (func == "011")
            this->decodedInstruction.setName("SLTIU");
        else if (func == "100")
            this->decodedInstruction.setName("XORI");
        else if (func == "110")
            this->decodedInstruction.setName("ORI");
        else if (func == "111")
            this->decodedInstruction.setName("ANDI");
        else if (func == "001")
        {
            this->decodedInstruction.setName("SLLI");
            imm = imm.substr(7, 5);
            this->decodedInstruction.setimm((int)std::stoul(imm, nullptr, 2));
        }
        else if (func == "101")
        {
            if (imm[1] == 1)
                this->decodedInstruction.setName("SRAI");
            else this->decodedInstruction.setName("SRLI");
            imm = imm.substr(7, 5);
            this->decodedInstruction.setimm((int)std::stoul(imm, nullptr, 2));
        }

    }
}

void Cpu::decodeSTypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string imm1 = "";
    std::string imm2 = "";
    std::string rs1 = "";
    std::string rs2 = "";
    std::string func = "";

    imm1 = rawBinary.substr(0, 7);
    rs2 = rawBinary.substr(7, 5);
    rs1 = rawBinary.substr(12, 5);
    func = rawBinary.substr(17, 3);
    imm2 = rawBinary.substr(20, 5);

    this->decodedInstruction.setrd("NA");

    int reg = std::stoul(rs1, nullptr, 2);
    this->decodedInstruction.setrs1(this->getIntReg(reg));
    reg = std::stoul(rs2, nullptr, 2);
    this->decodedInstruction.setrs2(this->getIntReg(reg));

    this->decodedInstruction.setFunc(func);
    //unsigned long temp = DataConversionHelper::getSignedIntegerFromBitStream(imm1 + imm2, 32);
    //int tempp = DataConversionHelper::getSignedIntegerFromBitStream(imm1 + imm2, 32);
    this->decodedInstruction.setimm(DataConversionHelper::getSignedIntegerFromBitStream(imm1 + imm2, 32));

    std::string opcode = this->decodedInstruction.getOpcode();
    if (opcode == "0100011")
    {
        if (func == "000")
            this->decodedInstruction.setName("SB");
        else if (func == "001")
            this->decodedInstruction.setName("SH");
        else if (func == "010")
            this->decodedInstruction.setName("SW");
        else if (func == "011")
        {
            this->decodedInstruction.setName("SD");
            this->decodedInstruction.setOperation("Store double");
        }
    }
    else if (opcode == "0100111")
    {
        reg = std::stoul(rs1, nullptr, 2);
        this->decodedInstruction.setrs1(this->getFloatReg(reg));
        reg = std::stoul(rs2, nullptr, 2);
        this->decodedInstruction.setrs2(this->getFloatReg(reg));

        if (func == "010")
            this->decodedInstruction.setName("FSW");
        else if (func == "011")
        {
            this->decodedInstruction.setName("FSD");
            this->decodedInstruction.setOperation("Store double");
        }
    }
    this->decodedInstruction.setWriteOption(true);
}

void Cpu::decodeRTypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string func7 = "";
    std::string rd = "";
    std::string rs1 = "";
    std::string rs2 = "";
    std::string func3 = "";

    func7 = rawBinary.substr(0, 7);
    rs2 = rawBinary.substr(7, 5);
    rs1 = rawBinary.substr(12, 5);
    func3 = rawBinary.substr(17, 3);
    rd = rawBinary.substr(20, 5);

    this->decodedInstruction.setimm(0);

    int reg = std::stoul(rs1, nullptr, 2);
    this->decodedInstruction.setrs1(this->getIntReg(reg));
    reg = std::stoul(rs2, nullptr, 2);
    this->decodedInstruction.setrs2(this->getIntReg(reg));
    reg = std::stoul(rd, nullptr, 2);
    this->decodedInstruction.setrd(this->getIntReg(reg));

    if (func7 == "0000000")
    {
        if (this->decodedInstruction.getOpcode() == "1010011")
        {
            this->decodedInstruction.setName("FADDS");
            this->decodedInstruction.setOperation("FP ALU op");
            int reg = std::stoul(rs1, nullptr, 2);
            this->decodedInstruction.setrs1(this->getFloatReg(reg));
            reg = std::stoul(rs2, nullptr, 2);
            this->decodedInstruction.setrs2(this->getFloatReg(reg));
            reg = std::stoul(rd, nullptr, 2);
            this->decodedInstruction.setrd(this->getFloatReg(reg));
        }
        else if (func3 == "000")
            this->decodedInstruction.setName("ADD");
        else if (func3 == "001")
            this->decodedInstruction.setName("SLL");
        else if (func3 == "010")
            this->decodedInstruction.setName("SLT");
        else if (func3 == "011")
            this->decodedInstruction.setName("SLTU");
        else if (func3 == "100")
            this->decodedInstruction.setName("XOR");
        else if (func3 == "101")
            this->decodedInstruction.setName("SRL");
        else if (func3 == "110")
            this->decodedInstruction.setName("OR");
        else if (func3 == "111")
            this->decodedInstruction.setName("AND");

    }
    else if (func7 == "0100000")
    {
        if (func3 == "000")
            this->decodedInstruction.setName("SUB");
        if (func3 == "101")
            this->decodedInstruction.setName("SRA");
    }
    this->decodedInstruction.setFunc(func7 + func3);

}


void Cpu::decodeBTypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string imm1 = "";
    std::string func3 = "";
    std::string rd = "";
    std::string rs1 = "";
    std::string rs2 = "";
    std::string imm2 = "";

    imm1 = rawBinary.substr(0, 7);
    rs2 = rawBinary.substr(7, 5);
    rs1 = rawBinary.substr(12, 5);
    func3 = rawBinary.substr(17, 3);
    imm2 = rawBinary.substr(20, 5);

    this->decodedInstruction.setrd("NA");
    if (func3 == "000")
        this->decodedInstruction.setName("BEQ");
    else if (func3 == "001")
        this->decodedInstruction.setName("BNE");
    else if (func3 == "111")
        this->decodedInstruction.setName("BGEU");
    else if (func3 == "110")
        this->decodedInstruction.setName("BLTU");
    else if (func3 == "100")
        this->decodedInstruction.setName("BLT");
    else if (func3 == "101")
        this->decodedInstruction.setName("BGE");

    int reg = std::stoul(rs1, nullptr, 2);
    this->decodedInstruction.setrs1(this->getIntReg(reg));
    reg = std::stoul(rs2, nullptr, 2);
    this->decodedInstruction.setrs2(this->getIntReg(reg));
    this->decodedInstruction.setFunc(func3);

    std::string imm = "";
    imm = imm1.substr(0, 1) + imm2.substr(4, 1) + imm1.substr(1, 6) + imm2.substr(0, 4);
    this->decodedInstruction.setimm(DataConversionHelper::getSignedIntegerFromBitStream(imm, 32));
}

void Cpu::decodeUTypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string imm = "";
    std::string rd = "";
    std::string opcode = this->decodedInstruction.getOpcode();

    imm = rawBinary.substr(0, 20);
    rd = rawBinary.substr(20, 5);

    int reg = std::stoul(rd, nullptr, 2);
    this->decodedInstruction.setrd(this->getIntReg(reg));
    if (opcode == "0110111")
    {
        this->decodedInstruction.setName("LUI");
    }
    else if (opcode == "0010111") {
        this->decodedInstruction.setName("AUIPC");
    }
    unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(imm, 32);
    int tempp = DataConversionHelper::getSignedIntegerFromBitStream(imm, 32);
    this->decodedInstruction.setimm(DataConversionHelper::getSignedIntegerFromBitStream(imm, 32));
}

void Cpu::decodeUJTypeInstruction()
{
    std::string rawBinary = this->decodedInstruction.getRawBinary();
    std::string imm = "";
    std::string rd = "";

    ////rawBinary[31..............0]
    ////      imm[20|10:1 |11|19:12] -- right most 20 bits of raw binary
    ////      imm[a |b    |c |d    ]
    ////      imm[20:1]
    ////implicit imm[0]=0 (LSB)

    ////rawBinary[31:0]
    ////rawBinary[31|21:12|22|30:23]

    ////for string, the indices will be reversed
    ////rawBinary[0:31]
    ////rawBinary[0 |10:19|9 |1:8]

    std::string ima = "";
    std::string imb = "";
    std::string imc = "";
    std::string imd = "";

    ima = rawBinary.substr(0, 1);
    imb = rawBinary.substr(12, 8);
    imc = rawBinary.substr(11, 1);
    imd = rawBinary.substr(1, 10); 
    rd = rawBinary.substr(20, 5);

    imm = ima + imb + imc + imd + "0";
    
    this->decodedInstruction.setimm(DataConversionHelper::getSignedIntegerFromBitStream(imm, 32));
    int reg = std::stoul(rd, nullptr, 2);
    this->decodedInstruction.setrd(this->getIntReg(reg));
    this->decodedInstruction.setName("JAL");
}

void Cpu::executeALUInstruction()
{
    if (this->executedALUInstruction.getState() == "decoded")
    {
        this->executedALUInstruction.setReqClockCycle(this->executedALUInstruction.calculateInstructionLatency(this->decodedInstruction) + 1);
        std::string instructionName = this->executedALUInstruction.getInstructionName();
        std::transform(instructionName.begin(), instructionName.end(), instructionName.begin(), ::tolower);

        if (instructionName == "add")
            this->add(this->executedALUInstruction.getrd(), this->executedALUInstruction.getrs1(), this->executedALUInstruction.getrs2());
        else if (instructionName == "addi")
            this->addi(this->executedALUInstruction.getrd(), this->executedALUInstruction.getrs1(), this->executedALUInstruction.getimm());
        else if (instructionName == "slli")
            this->slli(this->executedALUInstruction.getrd(), this->executedALUInstruction.getrs1(), this->executedALUInstruction.getimm());
        else if (instructionName == "auipc")
            this->auipc(this->executedALUInstruction.getrd(), this->executedALUInstruction.getimm());
        else if (instructionName == "blt")
            this->blt(this->executedALUInstruction.getrs1(), this->executedALUInstruction.getrs2(), this->executedALUInstruction.getimm());
        else if (instructionName == "jal")
            this->jal(this->executedALUInstruction.getrd(), this->executedALUInstruction.getimm());
        else if (instructionName == "jalr")
            this->jalr(this->executedALUInstruction.getrd(), this->executedALUInstruction.getrs1(), this->executedALUInstruction.getimm());
        else if (instructionName == "lui")
            this->lui(this->executedALUInstruction.getrd(), this->executedALUInstruction.getimm());
        else if (instructionName == "fadds")
            this->fadds(this->executedALUInstruction.getrs1(), this->executedALUInstruction.getrs2(), this->executedALUInstruction.getrd());

        this->executedALUInstruction.setState("complete");
        std::cout << "Execute   | Instruction name: " << this->executedALUInstruction.getInstructionName() << std::endl;

        this->pipelineStall = this->calculatePipelineStall(this->executedALUInstruction.getReqClockCycle());

    }
    /*this->decodeInstruction(clockTick);*/

    //this->scheduleNextEvent();
}

void Cpu::executeReadInstruction()
{
    if (this->memReadInstruction.getState() == "decoded")
    {
        this->memReadInstruction.setReqClockCycle(this->memReadInstruction.calculateInstructionLatency(this->decodedInstruction) + 1);
        std::string instructionName = this->memReadInstruction.getInstructionName();
        std::transform(instructionName.begin(), instructionName.end(), instructionName.begin(), ::tolower);

        if (instructionName == "lw")
            this->lw(this->memReadInstruction.getrd(), this->memReadInstruction.getimm(), this->memReadInstruction.getrs1());
        else if (instructionName == "flw")
            this->flw(this->memReadInstruction.getrs1(), this->memReadInstruction.getrd(), this->memReadInstruction.getimm());

        this->memReadInstruction.setState("complete");
        std::cout << "Execute (Read/Load)   | Instruction name: " << this->memReadInstruction.getInstructionName() << std::endl;
        this->pipelineStall = this->calculatePipelineStall(this->memReadInstruction.getReqClockCycle());
    }
}

void Cpu::executeWriteInstruction()
{
    if (this->memWriteInstruction.getState() == "decoded")
    {
        this->memWriteInstruction.setReqClockCycle(this->memWriteInstruction.calculateInstructionLatency(this->decodedInstruction) + 1);
        std::string instructionName = this->memWriteInstruction.getInstructionName();
        std::transform(instructionName.begin(), instructionName.end(), instructionName.begin(), ::tolower);

        if (instructionName == "sw")
            this->sw(this->memWriteInstruction.getrs2(), this->memWriteInstruction.getimm(), this->memWriteInstruction.getrs1());
        else if (instructionName == "fsw")
            this->fsw(this->memWriteInstruction.getrs1(), this->memWriteInstruction.getrs2(), this->memWriteInstruction.getimm());

        this->memWriteInstruction.setState("complete");
        std::cout << "Execute (Write/Store)   | Instruction name: " << this->memWriteInstruction.getInstructionName() << std::endl;
        this->pipelineStall = this->calculatePipelineStall(this->memWriteInstruction.getReqClockCycle());
    }
}

int Cpu::calculatePipelineStall(int reqClockCycles)
{
    return reqClockCycles == 1 ? 0 : reqClockCycles - 1;
}

void Cpu::addi(std::string rd, std::string rs1, int imm) {
    int rdIndex = this->getIntRegIndex(rd);
    int rs1Index = this->getIntRegIndex(rs1);
    
    this->setIntRegisterByIndex(rdIndex, DataConversionHelper::addToBitStream(this->getIntRegisterByIndex(rs1Index), imm));
}

void Cpu::add(std::string rd, std::string rs1, std::string rs2) {
    int rdIndex = this->getIntRegIndex(rd);
    int rs1Index = this->getIntRegIndex(rs1);
    int rs2Index = this->getIntRegIndex(rs2);

    // int rs1Value = (int)std::stoul(this->getIntRegisterByIndex(rs1Index),nullptr,2);
    // int rs2Value = (int)std::stoul(this->getIntRegisterByIndex(rs2Index),nullptr,2);
    int rs1Value = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);
    int rs2Value = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(rs2Index), 32);

    int rdValue = rs1Value + rs2Value;
    this->setIntRegisterByIndex(rdIndex, std::bitset<32>(rdValue).to_string());
}

void Cpu::slli(std::string rd, std::string rs1, int imm)
{
    int rdIndex = this->getIntRegIndex(rd);
    int rs1Index = this->getIntRegIndex(rs1);
    std::bitset<32> tempValue(this->getIntRegisterByIndex(rs1Index));
    tempValue = tempValue << imm;
    this->setIntRegisterByIndex(rdIndex, tempValue.to_string());
}

void Cpu::auipc(std::string rd, int imm) {
    //AUIPC forms a 32-bit offset from the 20-bit U-immediate, flling in the lowest 12 bits with
    //zeros, adds this offset to the pc, then places the result in register rd.

    //this->intRegisters[rdIndex] = (this->pc - 4) + (imm << 12);
    //(this->pc - 4)
    //retrieve entire address, 32 bit
    //convert to decimal
    //create 32 bit offset with imm and extra 12 0s
    //convert the offset to decimal
    //add offset to address
    //convert to std::bitset
    //store in rd

    std::bitset<32> offset = std::bitset<32>(imm);
    offset = offset << 12;

    std::bitset<13> tempPC(this->getPC());
    //this->verifyPC(pc);
    //tempPC = tempPC - 4;
    //std::string tempAddString = (tempPC + 3)->to_string() + (tempPC + 2)->to_string() + (tempPC + 1)->to_string() + (tempPC + 0)->to_string();

    long int newAddressValue = DataConversionHelper::getUnSignedLongIntegerFromBitStream(tempPC.to_string(), 32) + DataConversionHelper::getUnSignedLongIntegerFromBitStream(offset.to_string(), 32);

    this->setIntRegisterByIndex(this->getIntRegIndex(rd), std::bitset<32>(newAddressValue).to_string());
}

void Cpu::lw(std::string rd, int imm, std::string rs1)
{
    //port should be used for loading value from memory
    unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);
    int tempp = DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);
    long int newAddressValue = imm + DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);
    //std::bitset<32> entireAddress = std::bitset<32> (newAddressValue);

    //std::bitset<8> lsbAddress = std::bitset<8> (entireAddress.substr(0,8));

    //use d-port to get data
    Packet tempDataPacket;
    tempDataPacket.initialisePacket(4);
    this->memoryConnection->getDataByAddress(std::bitset<13>(newAddressValue).to_string(), 4, tempDataPacket);
    std::string word = "";
    for (int itr = 3; itr >= 0; --itr)
    {
        word += tempDataPacket.extractByteByIndex(itr);
    }
    //std::bitset<8> lsb = tempDataPacket.getDataLSB();

    //std::string word = tempDataPacket.getDataMSB() + tempDataPacket.getDataByte2() + tempDataPacket.getDataByte3() + tempDataPacket.getDataLSB();
    this->setIntRegisterByIndex(this->getIntRegIndex(rd), std::bitset<32>(word).to_string());

}

void Cpu::sw(std::string rs2, int imm, std::string rs1) {

    int rs2Index = this->getIntRegIndex(rs2);
    int rs1Index = this->getIntRegIndex(rs1);
    std::string registerValueString = this->getIntRegisterByIndex(rs2Index);
    Packet tempDataPacket;
    tempDataPacket.initialisePacket(4);
    std::vector<std::string> block = std::vector<std::string>(4, "");
    block.at(0) = registerValueString.substr(0, 8);
    block.at(1) = registerValueString.substr(8, 8);
    block.at(2) = registerValueString.substr(16, 8);
    block.at(3) = registerValueString.substr(24, 8);

    tempDataPacket.populateData(block);
   /* unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);
    int tempp = DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);*/
    long int newAddressValue = imm + DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);

    this->memoryConnection->setDataByAddress(std::bitset<13>(newAddressValue).to_string(), 4, tempDataPacket);
}

void Cpu::blt(std::string rs1, std::string rs2, int offset)
{
    //if rs1<rs2, jump to offset
    int rs1Index = this->getIntRegIndex(rs1);
    int rs2Index = this->getIntRegIndex(rs2);

    int rs1Value = (int)std::stoul(this->getIntRegisterByIndex(rs1Index), nullptr, 2);
    int rs2Value = (int)std::stoul(this->getIntRegisterByIndex(rs2Index), nullptr, 2);

    if (rs1Value < rs2Value)
    {
        // std::string tempAddString = (std::bitset<32> (offset));
        // tempAddString = tempAddString.substr(24,8);
        //std::bitset<8> tempPC = std::bitset<8> (tempAddString);
        //this->setPC(&tempPC);
        //*(this->pc) = tempPC;
        //this->pc = this->memoryConnection->getMemoryAddressByIndex((int)std::stoul(tempAddString,nullptr,2));
        this->pc = std::bitset<32>(offset).to_string();
        //this->decodeQueue.clear();
        this->decodedInstruction.setState("discarded");
        this->fetchedInstruction.setState("discarded");
        this->pcModified = true;
    }

}

void Cpu::jal(std::string rd, int imm)
{
    ////pc <- pc - 4
    ////rd <- pc
    //// pc = pc + sign-extended(imm) -- original implementation
    //// pc = imm -- in this implementation
    
    this->pc = DataConversionHelper::addToBitStream(this->pc, -4);
    this->setIntRegisterByIndex(this->getIntRegIndex(rd), this->pc);
    //this->decodeQueue.clear();
    this->pc = DataConversionHelper::convertTo32BitStreamFromInt(imm);
    this->decodedInstruction.setState("discarded");
    this->fetchedInstruction.setState("discarded");
    this->pcModified = true;
}

void Cpu::jalr(std::string rd, std::string rs1, int imm)
{

    std::bitset<13> newpc(this->getPC());
    //std::bitset<8> pc = this->getPC();

    //pc = pc - 4;
    //newpc = newpc - 4;

    int rs1Index = this->getIntRegIndex(rs1);
    int rs1Value = (int)std::stoul(this->getIntRegisterByIndex(rs1Index), nullptr, 2);

    //std::string tempAddString = (std::bitset<32> (imm + rs1Value));
    //long int newAddressValue = (std::stoul(tempAddString,nullptr,2) + imm) & 0xfffffffe;
    //this->setPC(std::bitset<32> (newAddressValue));
    //tempAddString = tempAddString.substr(24,8);
    //std::bitset<13> tempPC = std::bitset<32> (imm + rs1Value);
    // this->pc = this->memoryConnection->getMemoryAddressByIndex((int)std::stoul(tempAddString,nullptr,2));
    this->pc = std::bitset<13>(imm + rs1Value).to_string();
    //tempAddString = (pc + 7)->to_string() + (pc + 6)->to_string() + (pc + 5)->to_string() + (pc + 4)->to_string();
    newpc = std::bitset<13>(((int)std::stoul(newpc.to_string(), nullptr, 2)) + 4);
    this->setIntRegisterByIndex(this->getIntRegIndex(rd), newpc.to_string());
    this->decodedInstruction.setState("discarded");
    this->fetchedInstruction.setState("discarded");
    this->pcModified = true;
}

void Cpu::lui(std::string rd, int imm) {
    std::bitset<20> offset = std::bitset<20>(imm);
    std::string offsetStr = offset.to_string();
    offsetStr += "000000000000";
    this->setIntRegisterByIndex(this->getIntRegIndex(rd), offsetStr);
}

void Cpu::flw(std::string rs1, std::string rd, int imm)
{
    unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);
    int tempp = DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);
    long int newAddressValue = imm + DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(this->getIntRegIndex(rs1)), 32);

    Packet tempDataPacket(4);
    this->memoryConnection->getDataByAddress(std::bitset<13>(newAddressValue).to_string(), 4, tempDataPacket);
    std::string floatWordString = "";
    for (int itr = 3; itr >= 0; --itr)
    {
        floatWordString += tempDataPacket.extractByteByIndex(itr);
    }

    //std::bitset<32> floatWord = this->GetFloatFromBits(std::bitset<32> (floatWordString));
    this->setFloatRegisterByIndex(this->getFloatRegIndex(rd), floatWordString);
}

void Cpu::fsw(std::string rs1, std::string rs2, int imm)
{
    int rs2Index = this->getFloatRegIndex(rs2);
    int rs1Index = this->getFloatRegIndex(rs1);
    std::string registerValueString = this->getFloatRegisterByIndex(rs2Index);
    Packet tempDataPacket(4);
    std::vector<std::string> block;
    block.push_back(registerValueString.substr(0, 8));
    block.push_back(registerValueString.substr(8, 8));
    block.push_back(registerValueString.substr(16, 8));
    block.push_back(registerValueString.substr(24, 8));

    tempDataPacket.populateData(block);
    unsigned long temp = DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);
    int tempp = DataConversionHelper::getSignedIntegerFromBitStream(this->getIntRegisterByIndex(rs1Index), 32);
    long int newAddressValue = imm + DataConversionHelper::getUnSignedLongIntegerFromBitStream(this->getFloatRegisterByIndex(rs1Index), 32);

    this->memoryConnection->setDataByAddress(std::bitset<13>(newAddressValue).to_string(), 4, tempDataPacket);
}

void Cpu::fadds(std::string rs1, std::string rs2, std::string rd)
{
    int rdIndex = this->getFloatRegIndex(rd);
    int rs1Index = this->getFloatRegIndex(rs1);
    int rs2Index = this->getFloatRegIndex(rs2);

    float rs1Value = DataConversionHelper::getFloatFromBitStream(this->getFloatRegisterByIndex(rs1Index));
    float rs2Value = DataConversionHelper::getFloatFromBitStream(this->getFloatRegisterByIndex(rs2Index));
    float rdValue = rs1Value + rs2Value;
    this->setFloatRegisterByIndex(rdIndex, DataConversionHelper::getBitStreamFromFloat(rdValue, 32));
}

Packet Cpu::getDataPacket(std::string packetBaseAddress)
{
    Packet dataPacket;
    dataPacket.initialisePacket(4);
    this->fetchFromMemory(packetBaseAddress, 4, dataPacket);
    return dataPacket;
}

Packet Cpu::getInstructionPacket(std::string packetBaseAddress)
{
    Packet instructionPacket;
    instructionPacket.initialisePacket(4);
    this->fetchFromMemory(packetBaseAddress, 4, instructionPacket);
    return instructionPacket;
}

void Cpu::fetchFromMemory(std::string packetBaseAddress, int dataSizeInBytes, Packet& packet)
{
    this->memoryConnection->getDataByAddress(packetBaseAddress, dataSizeInBytes, packet);
}

std::string Cpu::convertTo32BitData(Packet packet)
{
    //
    std::string temp32BitDataString = "";
    for (int itr = 3; itr >= 0; --itr)
    {
        temp32BitDataString += packet.extractByteByIndex(itr);
    }
    //temp32BitDataString = packet.getDataMSB() + packet.getDataByte2() + packet.getDataByte3() + packet.getDataLSB();
    return temp32BitDataString;
}

std::string Cpu::extractOpcodeAsString(std::string rawBinary)
{
    std::string opcode = "";
    std::string rawBinaryString = rawBinary;
    opcode = rawBinaryString.substr(25, 7);
    return opcode;
}

void Cpu::cleanInstructionObject(Instruction *toBeCleaned)
{
    toBeCleaned->setFunc("");
    toBeCleaned->setimm(0);
    toBeCleaned->setName("");
    toBeCleaned->setOpcode("");
    toBeCleaned->setRawBinary("");
    toBeCleaned->setrd("");
    toBeCleaned->setReqClockCycle(0);
    toBeCleaned->setrs1("");
    toBeCleaned->setrs2("");
    toBeCleaned->setState("");
    toBeCleaned->setWriteOption(false);
    toBeCleaned->setMemoryReadOption(false);
    toBeCleaned->setType("");
    toBeCleaned->setOperation("");
}

void Cpu::initialisePipelineInstructions()
{
    this->fetchedInstruction = Instruction();
    this->decodedInstruction = Instruction();
    this->executedALUInstruction = Instruction();
    this->memWriteInstruction = Instruction();
    this->memReadInstruction = Instruction();

    this->cleanInstructionObject(&this->fetchedInstruction);
    this->cleanInstructionObject(&decodedInstruction);
    this->cleanInstructionObject(&executedALUInstruction);
    this->cleanInstructionObject(&memWriteInstruction);
    this->cleanInstructionObject(&memReadInstruction);
}

//void Cpu::storeBackAfterExecute(std::string address, std::vector<std::string> data)
//{
//    if (this->writeBackInstruction.getState() == "executed")
//    {
//        Packet tempDataPacket(4);
//        tempDataPacket.populateData(data);
//        this->memoryConnection->setDataByAddress(address, 4, tempDataPacket);
//        this->writeBackInstruction.setState("written");
//    }
//}

void Cpu::executePipelineEvent(long clockTick, Event* event)
{
    this->clockTick = clockTick;
    if (this->pipelineStall == 0)
    {
        if (event->getType() == "FetchInstructionEvent")
        {
            this->fetchInstruction();
            this->updatePC();
            /*if (!this->pcModified)
                this->updatePC();
            else
                this->pcModified = false;*/
        }
        else if (event->getType() == "DecodeInstructionEvent")
        {
            this->decodeInstruction();
        }
        else if (event->getType() == "ExecuteALUInstructionEvent")
        {
            this->executeALUInstruction();
            this->printInstructionStatement(this->executedALUInstruction);
        }
        else if (event->getType() == "MemoryReadInstructionEvent")
        {
            this->executeReadInstruction();
            this->printInstructionStatement(this->memReadInstruction);
        }
        else if (event->getType() == "MemoryWriteInstructionEvent")
        {
            this->executeWriteInstruction();
            this->printInstructionStatement(this->memWriteInstruction);
        }
        this->updatePipeline();
        
    }
    else
        --this->pipelineStall;
    
}

void Cpu::createAndScheduleNextPipelineEvent(long arrivalTime, std::string type)
{
    if (type == "fetch")
    {
        this->systemMEQ->push_back(new FetchInstructionEvent(arrivalTime, this->name));
    }
    else if (type == "decode")
    {
        this->systemMEQ->push_back(new DecodeInstructionEvent(arrivalTime, this->name));
    }
    else if (type == "execute")
    {
        this->systemMEQ->push_back(new ExecuteALUInstructionEvent(arrivalTime, this->name));
    }
    else if (type == "read")
    {
        this->systemMEQ->push_back(new MemoryReadInstructionEvent(arrivalTime, this->name));
    }
    else if (type == "write")
    {
        this->systemMEQ->push_back(new MemoryWriteInstructionEvent(arrivalTime, this->name));
    }
    std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t[ " << type << " event schduled, for t = "<< arrivalTime << " ]" << std::endl;
}

void Cpu::updatePipeline()
{
    if (this->decodedInstruction.getState() == "decoded")
    {
        ////update after decoded
        if (this->decodedInstruction.isWriteNeeded() && (this->memWriteInstruction.getState() == "complete" || this->memWriteInstruction.getState() == ""))
        {
            this->memWriteInstruction = this->decodedInstruction;
            this->cleanInstructionObject(&this->decodedInstruction);
            this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall  + 1, "write");
        }
        else if (this->decodedInstruction.isMemoryReadNeeded() && (this->memWriteInstruction.getState() == "complete" || this->memWriteInstruction.getState() == ""))
        {
            this->memReadInstruction = this->decodedInstruction;
            this->cleanInstructionObject(&this->decodedInstruction);
            this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "read");
        }
        else if (this->executedALUInstruction.getState() == "complete" || this->executedALUInstruction.getState() == "")
        {
            this->executedALUInstruction = this->decodedInstruction;
            this->cleanInstructionObject(&this->decodedInstruction);
            this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "execute");
        }
        if (this->fetchedInstruction.getState() == "" && this->isEndOfInstructionReached())
        {
            this->cleanInstructionObject(&this->decodedInstruction);
        }

    }
    ////update for next decode 
    if (this->fetchedInstruction.getState() == "fetched" && (this->decodedInstruction.getState() == "decoded" || this->decodedInstruction.getState() == ""))
    {
        this->decodedInstruction = this->fetchedInstruction;
        this->cleanInstructionObject(&this->fetchedInstruction);
        this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "decode");
        this->checkForProgram();
    }
    /*else if (this->fetchedInstruction.getState() == "")
    {
        this->cleanInstructionObject(&this->decodedInstruction);
    }*/
    if (this->decodedInstruction.getState() == "discarded")
    {
        this->cleanInstructionObject(&this->fetchedInstruction);
        this->cleanInstructionObject(&this->decodedInstruction);
        
        //remove the future pipeline events since jump is encountered
        std::cout << "\nBranch/Jump instruction encountered.\nDiscarding the decode and fetch events, and adding new fetch event." << std::endl;
        std::vector<Event*>::iterator from, to;
        from = this->systemMEQ->begin();
        ++from;
        to = this->systemMEQ->end();
        this->systemMEQ->erase(from, to);
        //add a new fetch event
        this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "fetch");
        std::cout << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tnew pc: " << DataConversionHelper::get32BitSignedIntegerFromBitStream(this->pc) << std::endl;
        //this->createAndScheduleNextPipelineEvent(this->clockTick + pipelineStall + 1, "decode");
    }
}

void Cpu::printInstructionStatement(Instruction instruction)
{
    std::string instructionType = instruction.getType();
    if (instructionType == "IType")
    {
        std::cout << instruction.getInstructionName() << " " << instruction.getrd() << ", " << instruction.getrs1() << ", " << instruction.getimm() << std::endl;
    }
    else if (instructionType == "RType")
    {
        //ADD rd,rs1,rs2
        std::cout << instruction.getInstructionName() << " " << instruction.getrd() << ", " << instruction.getrs1() << ", " << instruction.getrs2() << std::endl;
    }
    else if (instructionType == "SType")
    {
        //SW rs1,rs2,imm
        //SW rs2, imm(rs1)
        //std::cout << instruction.getInstructionName() << " " << instruction.getrs1() << ", " << instruction.getrs2() << ", " << instruction.getimm() << std::endl;
        std::cout << instruction.getInstructionName() << " " << instruction.getrs2() << ", " << instruction.getimm() << "(" << instruction.getrs1() << ")" << std::endl;
    }
    else if (instructionType == "BType")
    {
        //BLT rs1,rs2,imm
        std::cout << instruction.getInstructionName() << " " << instruction.getrs1() << ", " << instruction.getrs2() << ", " << instruction.getimm() << std::endl;
    }
    else if (instructionType == "UType")
    {
        //LUI rd,imm
        std::cout << instruction.getInstructionName() << " " << instruction.getrd() << ", " << instruction.getimm() << std::endl;
    }
    else if (instructionType == "UJType")
    {
        //JAL rd,imm
        std::cout << instruction.getInstructionName() << " " << instruction.getrd() << ", " << instruction.getimm() << std::endl;
    }
}

/*
//Load and store instructions transfer a value between the registersand memory.Loads are encoded in the I - type format and stores are S - type.
//The efective byte address is obtained by adding register rs1 to the sign - extended 12 - bit oset.Loads copy a value from memory to register rd.
//Stores copy the value in register rs2 to memory.

//The LW instruction loads a 32 - bit value from memory into rd.
//LH loads a 16 - bit value from memory, then sign - extends to 32 - bits before storing in rd.
//LHU loads a 16 - bit value from memory but then zero extends to 32 - bits before storing in rd.
//LB and LBU are defned analogously for 8 - bit values.

//The SD, SW, SH, and SB instructions store 64-bit, 32-bit, 16-bit, and 8-bit values from the low bits of register rs2 to memory.
*/

/*
Floating-point loads and stores use the same base+oset addressing mode as the integer base ISA,
with a base address in register rs1 and a 12-bit signed byte ofset.

The FLW instruction loads a single-precision floating-point value from memory into floating-point register rd.

FSW stores a single-precision value from floating-point register rs2 to memory.
*/