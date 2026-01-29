#include <iostream>
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

    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    errif(sockfd==-1,"create sockfd fail");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    serv_addr.sin_port=htons(8888);

    socklen_t serv_addr_len=sizeof(serv_addr);
    errif(connect(sockfd, (sockaddr*)&serv_addr, serv_addr_len)==-1, "connect socket fail");

    while (true) {
        char buf[1024];
        memset(buf,0, sizeof(buf));
        std::cin>>buf;
        ssize_t write_bytes=write(sockfd,buf,sizeof(buf));

        if (write_bytes==-1) {
            std::cout<<"socket already discoonectd,can't write more\n";
            break;
        }

        bzero(&buf, sizeof(buf));
        ssize_t read_bytes=read(sockfd, buf, sizeof(buf));

        if (read_bytes>0) {
            std::cout<<"message from server"<<":"<<buf;
        }else if (read_bytes==0) {
            std::cout<<"server disconnectd";
            break;
        }else if(read_bytes == -1){
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}