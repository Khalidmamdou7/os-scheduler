#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdlib.h>
#include "../ProcessStructs/ProcessData.h"


struct PriorQueueNode
{
    struct ProcessData pData;
    struct PriorQueueNode* next;
};


struct PriorQueue
{
    struct PriorQueueNode* front;
    struct PriorQueueNode* rear;
};

struct PriorQueue* createPriorQueue();


// Function to push according to priority
void enqueue(struct PriorQueue* q, struct ProcessData pData);
// Removes the element with the
// highest priority from the list
struct ProcessData dequeue(struct PriorQueue* q);
//Function to check is list is empty
int isEmpty(struct PriorQueue* q);
//Function to print the queue from highest to lowest
void printQueue(struct PriorQueue* q);
// Return the value at head
struct PriorQueueNode* peek(struct PriorQueue* q);
#endif