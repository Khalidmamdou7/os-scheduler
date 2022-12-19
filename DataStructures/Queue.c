#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

struct Queue* createQueue()
{
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

void enqueue(struct Queue* q, struct ProcessData pData)
{
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    newNode->pData = pData;
    newNode->next = NULL;
    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
        return;
    }
    q->rear->next = newNode;
    q->rear = newNode;
}

struct ProcessData dequeue(struct Queue* q)
{
    if (q->front == NULL)
    {
        struct ProcessData p;
        p.id = -1;
        return p;
    }
    struct QueueNode* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    struct ProcessData p = temp->pData;
    free(temp);
    return p;
}

struct QueueNode* peak(struct Queue* q)
{
    if (q->front == NULL)
        return NULL;
    return q->front;
}

int isEmpty(struct Queue* q)
{
    if (q->front == NULL)
        return 1;
    return 0;
}

void printQueue(struct Queue* q)
{
    struct QueueNode* temp = q->front;
    while (temp != NULL)
    {
        printf("Process %d arrived at %d and will run for %d with priority %d\n",
            temp->pData.id, temp->pData.arrivalTime, temp->pData.runningTime, temp->pData.priority);
        temp = temp->next;
    }
}