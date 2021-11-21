#pragma once
#include "event.h"
class MemoryWriteInstructionEvent :
	public Event
{
public:
	MemoryWriteInstructionEvent() { this->setTypeID("pipeline event"); };
	MemoryWriteInstructionEvent(long arrivalTime, std::string cpuID) : Event(arrivalTime, cpuID) { this->setTypeID("pipeline event"); };
	std::string getType() { return "MemoryWriteInstructionEvent"; };
};

