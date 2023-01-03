#ifndef PROCESSDATA_H
#define PROCESSDATA_H

struct ProcessData
{
    int id;
    int arrivalTime;
    int runningTime;
    int priority;
    int memsize;
    int memstart=-1;
};

#endif