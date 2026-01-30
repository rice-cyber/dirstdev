#include "InetAddress.h"
#include "Socket.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"

#define BUFFER_SIZE 1024 

int main(){
    InetAddress *serv_addr=new InetAddress("127.0.0.1",8888);
    Socket *serv_sock=new Socket();
    serv_sock->connect(serv_addr);
    delete serv_addr;
    while (true) {
        char buf[BUFFER_SIZE];  //在这个版本，buf大小必须大于或等于服务器端buf大小，不然会出错，想想为什么？
        bzero(&buf, sizeof(buf));
        scanf("%s", buf);
        ssize_t write_bytes = write(serv_sock->getFd(), buf, strlen(buf)+1);
        if(write_bytes == -1){
            printf("socket already disconnected, can't write any more!\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(serv_sock->getFd(), buf, sizeof(buf));
        if(read_bytes > 0){
            printf("message from server: %s\n", buf);
        }else if(read_bytes == 0){
            printf("server socket disconnected!\n");
            break;
        }else if(read_bytes == -1){
            close(serv_sock->getFd());
            errif(true, "socket read error");
        }
    }
    close(serv_sock->getFd());
    delete serv_sock;
    return 0;
    }

    