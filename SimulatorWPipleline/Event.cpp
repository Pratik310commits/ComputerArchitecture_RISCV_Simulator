#include "event.h"

Event::Event(void) : arrivalTime(999999), priority(999999) {}

Event::Event(long arrivalTime, std::string cpuID) : arrivalTime(arrivalTime), cpuID(cpuID){}

//Event newEvent(randomNumber, value, randomNumber);
Event::Event(long time, long priority)
{
    this->arrivalTime = time;
    this->priority = priority;
}

//Returns the arrival time of the event
long Event::getArrivalTime()
{
    return this->arrivalTime;
}

//Returns the priority the event
long Event::getPriority()
{
    return this->arrivalTime;
}

//Sets the arrival time of the event
void Event::setArrivalTime(long time)
{
    this->arrivalTime = time;
}

//Sets the priority of the event
void Event::setPriority(long priority)
{
    this->priority = priority;
}

std::string Event::getAssignedCPUID()
{
    return this->cpuID;
}

void Event::setCPUID(std::string cpuID)
{
    this->cpuID = cpuID;
}

std::string Event::getTypeID()
{
    return this->typeID;
}

void Event::setTypeID(std::string typeID)
{
    this->typeID = typeID;
}

std::string Event::getType()
{
    return "Event";
}
