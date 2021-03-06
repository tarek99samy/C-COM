#include "headers/headers.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "headers/message_buffer.h"
int remainingtime;

int main(int agrc, char *argv[])
{

    initClk();
    //printf("start process\n");
    key_t key_id;
    int msgq_id1, msgq_id2, send_val;

    key_id = ftok("keyfile", 65);
    msgq_id1 = msgget(key_id, 0666 | IPC_CREAT);
    msgq_id2 = msgget(key_id + 6, 0666 | IPC_CREAT);
    struct msgbuff message;
    message.mtype = getpid();
    strcpy(message.mtext, "stop");
    remainingtime = atoi(argv[1]);
    while (remainingtime > 0)
    {
        //printf("waiting\n");
        message.mtype = getpid();
        msgrcv(msgq_id1, &message, sizeof(message.mtext), message.mtype, !IPC_NOWAIT);
        int lastTime = getClk();
        while (getClk() - lastTime < 1)
        {
        }
        lastTime = getClk();
        remainingtime -= 1;
        message.mtype = getppid();
        msgsnd(msgq_id2, &message, sizeof(message.mtext), !IPC_NOWAIT);
        message.mtype = getpid();
        //printf("send message \n");
    }
    destroyClk(false);
    //printf("end process\n");
    exit(0);
}
