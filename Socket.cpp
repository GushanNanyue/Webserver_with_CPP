//
// Created by cl on 2025/9/22.
//

#include "Socket.h"

#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

Socket::Socket()
    : sock_fd_(-1)
{
    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd_ == -1) {
        perror("socket create error");
    }
}

Socket::Socket(int sock_fd)
    : sock_fd_(sock_fd)
{
    if (sock_fd == -1) {
        perror("socket create error");
    }
}

Socket::~Socket() {
    if(sock_fd_ != -1){
        close(sock_fd_);
        sock_fd_ = -1;
    }
}

void Socket::bind(NetAddress *addr) {
    if (::bind(sock_fd_,reinterpret_cast<sockaddr *>(&addr->address_),addr->address_length_) < 0) {
        perror("bind error");
    }
}

void Socket::listen() {
    if (::listen(sock_fd_,SOMAXCONN) < 0) {
        perror("listen error");
    }
}

void Socket::set_nonblocking() {
    int flag = fcntl(sock_fd_,F_GETFL,0);
    if (flag == -1) {
        perror("fcntl failed");
    }
    if (fcntl(sock_fd_,F_SETFL,flag | O_NONBLOCK) == -1)
        perror("fcntl failed");
}

int Socket::accept(NetAddress* address) {
    int conn_fd = ::accept(sock_fd_,reinterpret_cast<sockaddr *>(&address->address_),&address->address_length_);
    return conn_fd;
}

int Socket::get_fd() {
    return sock_fd_;
}



