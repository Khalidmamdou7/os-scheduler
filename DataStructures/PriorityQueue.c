#include <stdio.h>
#include <stdlib.h>
#include "PriorityQueue.h"


struct PriorQueue* createPriorQueue()
{
    struct PriorQueue* q = (struct PriorQueue*)malloc(sizeof(struct PriorQueue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}



void Priorenqueue(struct PriorQueue* q, struct ProcessData pData)
{
    struct PriorQueueNode* newNode = (struct PriorQueueNode*)malloc(sizeof(struct PriorQueueNode));
    newNode->pData = pData;
    newNode->next = NULL;
    struct PriorQueueNode* start=q->front;
    if(q->front->pData.priority>newNode->pData.priority)
    {

        newNode->next=q->front;
        q->front=newNode;
    }
    else
    {
        while(start->next!=NULL&&start->next->pData.priority<newNode->pData.priority)
        {
        start=start->next;
        }
        newNode->next=start->next;
        start->next=newNode;
    }

}
struct ProcessData Priordequeue(struct PriorQueue* q)
{
  struct PriorQueueNode* start=q->front;
  q->front=q->front->next;
  free(start);

}


int PriorisEmpty(struct PriorQueue* q)
{
    if (q->front == NULL)
        return 1;
    return 0;
}


void PriorprintQueue(struct PriorQueue* q)
{
    struct PriorQueueNode* temp = q->front;
    while (temp != NULL)
    {
        printf("Process %d arrived at %d and will run for %d with priority %d\n",
            temp->pData.id, temp->pData.arrivalTime, temp->pData.runningTime, temp->pData.priority);
        temp = temp->next;
    }
}



struct PriorQueueNode* peek(struct PriorQueue* q)
{
    if (q->front == NULL)
        return NULL;
    return q->front;
}