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



void Priorenqueue(struct PriorQueue* q, struct ProcessData pData,int priority)
{
    struct PriorQueueNode* newNode = (struct PriorQueueNode*)malloc(sizeof(struct PriorQueueNode));
    newNode->pData = pData;
    newNode->priority=priority;
    newNode->next = NULL;
    struct PriorQueueNode* start=q->front;
    if(PriorisEmpty(q))
    {
        q->front=newNode;
        return;
    }
    if(q->front->priority  >  newNode->priority)
    {

        newNode->next=q->front;
        q->front=newNode;
    }
    else
    {
        while(start->next!=NULL&&start->next->priority <  newNode->priority)
        {
        start=start->next;
        }
        newNode->next=start->next;
        start->next=newNode;
    }

}
struct ProcessData Priordequeue(struct PriorQueue* q)
{
    if (q->front == NULL)
    {
        struct ProcessData p;
        p.id = -1;
        return p;
    }
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
        printf("Process %d arrived at %d and will run for %d with priority %d actual priority %d  \n",
            temp->pData.id, temp->pData.arrivalTime, temp->pData.runningTime, temp->pData.priority,temp->priority);
        temp = temp->next;
    }
}



struct PriorQueueNode* peek(struct PriorQueue* q)
{
    if (q->front == NULL)
        return NULL;
    return q->front;
}
// int main()
// { 
//     struct PriorQueue* s=createPriorQueue();
//     printf("1 empty 0 not empty %d : \n",PriorisEmpty(s));
//     struct ProcessData pData;
//     pData.priority=6;
//     Priorenqueue(s,pData,6);
//     pData.priority=4;
//     Priorenqueue(s,pData,4);
//     pData.priority=7;
//     Priorenqueue(s,pData,7);
//     printf("1 empty 0 not empty %d : \n",PriorisEmpty(s));
//     PriorprintQueue(s);
//     Priordequeue(s);
//     printf("--------------------------------- \n");
//     PriorprintQueue(s);
//     printf("--------------------------------- \n");
//     Priordequeue(s);
//     printf("--------------------------------- \n");
//     PriorprintQueue(s);
//      Priordequeue(s);
//     printf("--------------------------------- \n");
//     PriorprintQueue(s);
//      Priordequeue(s);
//     printf("--------------------------------- \n");
//     PriorprintQueue(s);

            











// }