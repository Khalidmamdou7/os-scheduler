#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "../ProcessStructs/ProcessData.h"


struct PriorQueueNode
{
    struct ProcessData pData;
    struct PriorQueueNode* next;
    int priority;
};


struct PriorQueue
{
    struct PriorQueueNode* front;
    struct PriorQueueNode* rear;
};

struct PriorQueue* createPriorQueue();


// Function to push according to priority
void Priorenqueue(struct PriorQueue* q, struct ProcessData pData,int priority);
// Removes the element with the
// highest priority from the list
struct ProcessData Priordequeue(struct PriorQueue* q);
//Function to check is list is empty
int PriorisEmpty(struct PriorQueue* q);
//Function to print the queue from highest to lowest
void PriorprintQueue(struct PriorQueue* q);
// Return the value at head
struct PriorQueueNode* peek(struct PriorQueue* q);
#endif