//
// Created by cl on 2025/9/23.
//

#include "Server.h"

#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#include "Channel.h"
#include "Epoll.h"
#include "EventLoop.h"
#include "NetAddress.h"
#include "Socket.h"

const static int READ_BUFFER_SIZE = 1024;
const static char* HTTP_RESPONSE =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1> Hello World </h1>";


Server::Server(EventLoop *loop)
    : loop_(loop)
{
    // “老三样”
    // 创建 server socket
    Socket *server_socket = new Socket();
    // 创建 server addr
    NetAddress *server_addr = new NetAddress("127.0.0.1",8000);
    // bind
    server_socket->bind(server_addr);
    // listen
    server_socket->listen();
    server_socket->set_nonblocking();

    // 创建 channel 关联 epoll 和 fd
    Channel* server_channel = new Channel(loop,server_socket->get_fd());
    // 设置回调函数
    std::function<void()> cb = [this, server_socket] { handle_new_connection(server_socket); };
    server_channel->set_callback(cb);
    // 将 channel 设置为监听可读事件
    server_channel->enable_reading();

}

Server::~Server() {

}

void Server::handle_new_connection(Socket *socket) {
    // 关闭时未 delete 有内存泄漏风险，后续解决
    NetAddress *client_addr = new NetAddress();
    Socket *client_socket = new Socket(socket->accept(client_addr));
    printf("new client fd %d! IP: %s Port: %d\n", client_socket->get_fd(), inet_ntoa(client_addr->address_.sin_addr), ntohs(client_addr->address_.sin_port));
    client_socket->set_nonblocking();

    // 创建客户端 channel
    Channel* client_channel = new Channel(loop_,client_socket->get_fd());
    // 设置回调函数
    std::function<void()> cb = [this, capture0 = client_socket->get_fd(), capture1 = loop_->get_epoll()] { handle_read_event(capture0, capture1); };
    client_channel->set_callback(cb);
    // 将 channel 设置为监听可读事件
    client_channel->enable_reading();
}

void Server::handle_read_event(int fd,Epoll* ep) {
    while (true) {
        char buf[READ_BUFFER_SIZE];
        ssize_t read_size = recv(fd, buf, READ_BUFFER_SIZE, 0);
        if (read_size > 0) {
            buf[read_size] = '\0';  // 防止脏数据
            printf("message from client fd %d: %s\n", fd, buf);

            // 返回HTTP 数据
            size_t write_len = strlen(HTTP_RESPONSE);
            const char *write_buf = HTTP_RESPONSE;
            while (write_len > 0) {
                ssize_t w = send(fd, write_buf, write_len, 0);
                if (w < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        break;
                    }
                    perror("send failed");
                    break;
                }
                write_len -= w;
                write_buf += w;
            }
        }
        else if (read_size < 0) {
            if (errno != EAGAIN && errno != EWOULDBLOCK) {
                perror("recv");
                ep->remove_event(fd);
                close(fd);
            }
            break;
        }
        else if (read_size == 0) {
            printf("client fd %d connection closed\n", fd);
            ep->remove_event(fd);
            close(fd);
            break;
        }
    }
}

