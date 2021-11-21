#pragma once
#include "event.h"
class DecodeInstructionEvent :
	public Event
{
public:
	DecodeInstructionEvent() { this->setTypeID("pipeline event"); };
	DecodeInstructionEvent(long arrivalTime, std::string cpuID) : Event(arrivalTime, cpuID) { this->setTypeID("pipeline event"); };
	std::string getType() { return "DecodeInstructionEvent"; };
};

