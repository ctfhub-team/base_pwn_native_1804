#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>


int main(int argc, char** argv){
    int  listenfd, connfd, n;
    struct sockaddr_in  servaddr;
    char  buff[4096];
    
    if( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ) return 0;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6666);

    if( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) return 0;
    if( listen(listenfd, 10) == -1) return 0;

    while(1){
        if( (connfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1)
            continue;
        n = send(connfd,"Welcome to CTFHUB\n", 18, 0); 
        while((n = recv(connfd, buff, 4096, 0)) > 0)
        {
            buff[n] = '\0';
            printf("%s",buff);fflush(stdout);
            if(strcmp(buff, "flag") > 0){
                printf("orzzz");fflush(stdout);
                FILE *fp = NULL;
                fp = fopen("./flag", "r");
                if (fp == NULL) {
                    send(connfd,"Pwn error...\n",13,0);
                    break;
                }
                fgets(buff, 512, fp);
                send(connfd,buff,strlen(buff),0);
                fclose(fp);
                break;
            } else {
                if(send(connfd,buff,n,0)<0) break;
            }
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}
