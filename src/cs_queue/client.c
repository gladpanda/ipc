#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf //消息结构体
{
    long types;
    char m[20];
};
struct msgbuf mymsgbuf, recvbuf; //定义消息结构变量

int writelog(char *message)
{
    FILE *fptr;

    fptr = fopen("logs.log", "a+");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr, "%s", message);
    fclose(fptr);
    return 0;
}

int main(int argc, const char *argv[])
{
    key_t key;
    int msgid;

    mymsgbuf.types = 200; //给消息结构赋值

    char sen[100] = "Client receive message from Server.\n";

    key = ftok("./abc", 'a'); //建立key值
    if (key < 0)
    {
        perror("ftok fail ");
        exit(1);
    }

    msgid = msgget(key, IPC_CREAT | IPC_EXCL | 0666);
    if (msgid < 0)
    {
        if (errno == EEXIST)
        {
            msgid = msgget(key, 0666);
        }
        else //其他错误退出
        {
            perror("msgget fail ");
            exit(1);
        }
    }
    while (1)
    {
        //接收消息
        msgrcv(msgid, &recvbuf, sizeof(mymsgbuf) - sizeof(long), 100, 0);
        if (strstr(recvbuf.m, "quit") != NULL)
        {
            break;
        }
        printf("receive message from A: %s", recvbuf.m); //
        writelog(sen);
        sleep(2);
    }

    //删除消息队列
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
