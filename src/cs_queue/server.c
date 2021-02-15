#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msgbuf 
{
  long types;
  char msg[20];
};
struct msgbuf mymsgbuf; 

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

  char sen[100] = "A send message to B and  C.\n";

  mymsgbuf.types = 200;

  key = ftok("./abc", 'a'); //建立key�?
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
    else 
    {
      perror("msgget fail ");
      exit(1);
    }
  }
  while (1) 
  {
    fgets(mymsgbuf.msg, 10, stdin);
    mymsgbuf.types = 100;
    writelog(sen);
    msgsnd(msgid, &mymsgbuf, sizeof(mymsgbuf) - sizeof(long), 0);
    if (strstr(mymsgbuf.msg, "quit") != NULL)
    {
      break;
    }
    sleep(1);
  }

  msgctl(msgid, IPC_RMID, NULL);

  return 0;
}
