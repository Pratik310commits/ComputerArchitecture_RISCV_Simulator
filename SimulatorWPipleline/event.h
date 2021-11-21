#pragma once
#include <string>
#include <iostream>

#ifndef _EVENT_H
#define _EVENT_H

class Event
{
private:
    long arrivalTime;
    long priority;
    std::string cpuID;
    std::string typeID;

public:

    Event(void);

    Event(long arrivalTime, std::string cpuID);

    Event(long time, long priority);

    long getArrivalTime();

    long getPriority();

    std::string getAssignedCPUID();

    std::string getTypeID();

    void setArrivalTime(long time);

    void setPriority(long priority);

    void setCPUID(std::string cpuID);

    void setTypeID(std::string typeID);

    virtual std::string getType();
    //virtual void schedule() = 0;

};
#endif



