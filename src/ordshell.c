#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
 
int argc  = 0;
char* argv[8] = {}; 
 
void string_get(char* buf){ 
    argc = 0;
    int i = 0;
    int status = 0;
    for(; buf[i]!=0; i++){
        if(!isspace(buf[i])&&status == 0){ 
            argv[argc++] = buf + i;
            status = 1;
        }else if(isspace(buf[i])){ 
            buf[i] = 0;
            status = 0;
        }   
    }   
}

int main(){
    char buf[1024] = {};
    while(1){
        memset(buf, 0x00, sizeof(buf));
        printf("please input your order:");
        scanf("%[^\n]", buf);  
        scanf("%*c");
        if(strcmp(buf, "exit") == 0)
        {
            exit(0);
            break;  
        }

        string_get(buf);

        int pid = fork(); 
        if(pid == -1){
            perror("fork");
        }
        else if(pid > 0)
            wait(NULL); 
            else{ 
                if(execvp(argv[0],argv) == -1)
                    perror("execvp"),exit(1);
                }
        }
    return 0;
}

