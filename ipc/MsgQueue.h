#ifndef MSGQUEUE_H
#define MSGQUEUE_H

#include <sys/ipc.h>
#include "MsgStruct.h"



int getMsgQueue(key_t key);
void sendMsg(int msgid, struct MsgStruct msg);
struct MsgStruct receiveMsg(int msgid, long mtype);
int isMsgQueueEmpty(int msgid);
void destroyMsgQueue(int msgid);



#endif