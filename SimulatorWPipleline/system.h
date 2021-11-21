#pragma once
#include "systemConfig.h"
class System
{
private:
	SystemConfig systemConfig;
	long clockTick;
	std::vector<Event*> MEQ;
	Cpu* cpu;
	MainMemory* ram;
public:
	System() {};
	void configureSystem(SystemConfig systemConfig);
	void handleMEQEvent();
	void run();
	void initialiseSystem();
	void initProgramCodeForCPU(std::vector<std::string>* program);
};

