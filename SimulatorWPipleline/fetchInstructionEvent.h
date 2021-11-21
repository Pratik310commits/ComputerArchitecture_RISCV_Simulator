#pragma once
#include "event.h"
class FetchInstructionEvent :
	public Event
{
public:
	FetchInstructionEvent() { this->setTypeID("pipeline event"); };
	FetchInstructionEvent(long arrivalTime, std::string cpuID) : Event(arrivalTime, cpuID) { this->setTypeID("pipeline event"); };
	std::string getType() { return "FetchInstructionEvent"; };
};

