#ifndef MSGSTRUCT_H
#define MSGSTRUCT_H

#include "../ProcessStructs/ProcessData.h"

struct MsgStruct
{
    long mtype;
    struct ProcessData data;
};

#endif