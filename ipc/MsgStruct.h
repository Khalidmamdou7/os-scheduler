#include "../ProcessStructs/ProcessData.h"

#ifndef MSGSTRUCT_H
#define MSGSTRUCT_H


struct MsgStruct
{
    long mtype;
    struct ProcessData data;
};

#endif