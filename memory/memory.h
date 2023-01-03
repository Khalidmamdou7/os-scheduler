#ifndef MEMORY_H
#define MEMORY_H
#include "../ProcessStructs/ProcessData.h"
#include "../DataStructures/Tree.h"



bool memory_allocation(struct ProcessData process, struct TreeNode* t,int* done, int* begin, int* end);
bool memory_deallocation(struct ProcessData process, struct TreeNode* t,int* done, int* begin, int* end);

#endif