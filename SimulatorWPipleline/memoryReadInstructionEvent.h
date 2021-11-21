#pragma once
#include "event.h"
class MemoryReadInstructionEvent :
	public Event
{
public:
	MemoryReadInstructionEvent() { this->setTypeID("pipeline event"); };
	MemoryReadInstructionEvent(long arrivalTime, std::string cpuID) : Event(arrivalTime, cpuID) { this->setTypeID("pipeline event"); };
	std::string getType() { return "MemoryReadInstructionEvent"; };
};

