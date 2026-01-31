#include "Socket.h"
#include "InetAddress.h"
#include <sys/socket.h>
#include "util.h"
#include <unistd.h>
#include <fcntl.h>



Socket::Socket() : fd(-1){
    fd= socket(AF_INET,SOCK_STREAM,0);
    errif(fd==-1, "create socket fail");
}

Socket::Socket(int _fd) : fd(_fd){
    errif(fd==-1, "create socket fail");
} 

Socket::~Socket(){
    if (fd!=-1) {
        close(fd);
        fd=-1;
    }
}

void Socket::bind(InetAddress *addr){
    errif(::bind(fd, (sockaddr*)&addr->addr, addr->addr_len)==-1,"socket bind fail" );
}

void Socket::listen(){
    errif(::listen(fd, SOMAXCONN)==-1, "listen socket fail");
}

void Socket::setnonblocking(){
    fcntl(fd,F_SETFD,(fcntl(fd, F_GETFD)) | O_NONBLOCK);
}

void Socket::connect(InetAddress *addr){
    errif(::connect(fd,(sockaddr*)&addr->addr,addr->addr_len)==-1, "sock connect fail");
}

int Socket::accept(InetAddress *addr){
    int clnt_fd=::accept(fd, (sockaddr*)&addr->addr, (socklen_t*)&addr->addr_len);
    errif(clnt_fd==-1, "accpt socket fail");
    return  clnt_fd;
}

int Socket::getFd(){
    return fd;
}
