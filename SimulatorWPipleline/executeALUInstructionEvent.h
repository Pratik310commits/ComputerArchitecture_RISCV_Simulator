#pragma once
#include "event.h"
class ExecuteALUInstructionEvent :
	public Event
{
public:
	ExecuteALUInstructionEvent() { this->setTypeID("pipeline event"); };
	ExecuteALUInstructionEvent(long arrivalTime, std::string cpuID) : Event(arrivalTime, cpuID) { this->setTypeID("pipeline event"); };
	std::string getType() { return "ExecuteALUInstructionEvent"; };
};

