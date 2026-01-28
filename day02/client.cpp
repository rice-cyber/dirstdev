#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "util.h"


int main(){

    int socketfd=socket(AF_INET, SOCK_STREAM, 0);
    errif(socketfd==-1, "create socked fail");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8888);

    errif(connect(socketfd, (sockaddr*)&serv_addr, sizeof(serv_addr)), "socket connect fail");

    while(true){
        char buf[1024];
        bzero(&buf,sizeof(buf));
        scanf("%s",buf);
        ssize_t write_bytes=write(socketfd, buf,sizeof(buf));
        if (write_bytes==-1) {
            printf("write socket fd %d fail",socketfd);
            break;
        }
        bzero(&buf,sizeof(buf));
        ssize_t read_bytes=read(socketfd,buf,sizeof(buf));
        if (read_bytes>0) {
            printf("message from server:%s\n",buf);
        }else if (read_bytes==0) {
            printf("socket disconnected\n");
            break;
        }else if (read_bytes==-1) {
            printf("read socket error\n");
            close(socketfd);
        }
    }
    close(socketfd);
    return 0;
}