#ifndef QUEUE_H
#define QUEUE_H

#include "ProcessData.h"

struct QueueNode
{
    struct ProcessData pData;
    struct QueueNode* next;
};


struct Queue
{
    struct QueueNode* front;
    struct QueueNode* rear;
};

struct Queue* createQueue();
void enqueue(struct Queue* q, struct ProcessData pData);
struct ProcessData dequeue(struct Queue* q);
int isEmpty(struct Queue* q);
void printQueue(struct Queue* q);
struct QueueNode* peak(struct Queue* q);

#endif