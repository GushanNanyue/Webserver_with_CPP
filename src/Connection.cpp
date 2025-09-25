//
// Created by cl on 2025/9/24.
//

#include "Connection.h"

#include <cstring>
#include <unistd.h>

#include "Channel.h"
#include "Socket.h"

const static int READ_BUFFER_SIZE = 1024;
const static char* HTTP_RESPONSE =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1> Hello World </h1>";


Connection::Connection(EventLoop * loop, Socket * socket )
    : socket_(socket),loop_(loop)
{
    channel_ = new Channel(loop,socket->get_fd());
    std::function cb = [this,program0 = socket->get_fd()]{handle_read_event(program0);};
    channel_->set_callback(cb);
    channel_->enable_reading();
}

Connection::~Connection() {
    delete channel_;
}

void Connection::handle_read_event(int fd) {
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
                delete_connection_callback_(socket_);
            }
            break;
        }
        else if (read_size == 0) {
            printf("client fd %d connection closed\n", fd);
            delete_connection_callback_(socket_);
            break;
        }
    }
}

void Connection::set_delete_connection_callback(std::function<void(Socket *)> cb) {
    delete_connection_callback_ = std::move(cb);
}


