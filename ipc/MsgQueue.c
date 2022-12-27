#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

#include "MsgStruct.h"

int getMsgQueue(key_t key)
{
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1)
    {
        perror("Error::MsgQueue::getMsgQueue: Error creating message queue");
        exit(1);
    }
    return msgid;
}

void sendMsg(int msgid, struct MsgStruct msg)
{
    if (msgsnd(msgid, &msg, sizeof(msg.data), 0) == -1)
    {
        perror("Error::MsgQueue::sendMsg: Error sending message");
        exit(1);
    }
}

struct MsgStruct receiveMsg(int msgid, long mtype)
{
    struct MsgStruct msg;
    if (msgrcv(msgid, &msg, sizeof(msg.data), mtype, 0) == -1)
    {
        perror("Error::MsgQueue::receiveMsg: Error receiving message");
        exit(1);
    }
    return msg;
}

int isMsgQueueEmpty(int msgid)
{
    struct msqid_ds msgQueueInfo;
    if (msgctl(msgid, IPC_STAT, &msgQueueInfo) == -1)
    {
        perror("Error::MsgQueue::isMsgQueueEmpty: Error getting message queue info");
        exit(1);
    }
    return msgQueueInfo.msg_qnum == 0;
}

void destroyMsgQueue(int msgid)
{
    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("Error::MsgQueue::destroyMsgQueue: Error destroying message queue");
        exit(1);
    }
    printf("Message queue destroyed successfully\n");
}
