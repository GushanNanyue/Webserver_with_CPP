#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <vector>
#include <cerrno>
#include <unordered_map>
#include <arpa/inet.h>

#include "Epoll.h"
#include "Socket.h"


const static int MAX_EVENTS = 1024;
const static int READ_BUFFER_SIZE = 1024;
const static char* HTTP_RESPONSE =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1> Hello World </h1>";


std::unordered_map<int, Socket*> clients;
std::unordered_map<int, Net_Address*> client_addresses;

void handle_read_event(int fd, Epoll* ep,
                       std::unordered_map<int, Socket*>& clients_map,
                       std::unordered_map<int, Net_Address*>& client_addresses_map) {
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

                // 回收资源
                delete clients_map[fd];
                delete client_addresses_map[fd];
                clients_map.erase(fd);
                client_addresses_map.erase(fd);
            }
            break;
        }
        else if (read_size == 0) {
            printf("client fd %d connection closed\n", fd);
            ep->remove_event(fd);
            close(fd);

            // 回收资源
            delete clients_map[fd];
            delete client_addresses_map[fd];
            clients_map.erase(fd);
            client_addresses_map.erase(fd);
            break;
        }
    }
}




int main() {
    // “老三样”
    // 创建 server socket
    Socket *server_socket = new Socket();
    // 创建 server addr
    Net_Address *server_addr = new Net_Address("127.0.0.1",8000);
    // bind
    server_socket->bind(server_addr);
    // listen
    server_socket->listen();
    server_socket->set_nonblocking();
    // 创建 epoll
    Epoll *ep = new Epoll(MAX_EVENTS);

    // 将 server_fd 加入监听事件队列
    ep->add_event(server_socket->get_fd(), EPOLLIN);
    while(true) {
        // 获取事件队列
        auto events = ep->wait_events(-1);
        // 遍历一个个处理
        for (auto &ev:events) {
            int fd = ev.data.fd;
            // 新连接
            if (fd == server_socket->get_fd()) {
                Net_Address *client_addr = new Net_Address();
                Socket *client_socket = new Socket(server_socket->accept(client_addr));
                printf("new client fd %d! IP: %s Port: %d\n", client_socket->get_fd(), inet_ntoa(client_addr->address_.sin_addr), ntohs(client_addr->address_.sin_port));
                client_socket->set_nonblocking();
                ep->add_event(client_socket->get_fd(), EPOLLIN | EPOLLET);

                // 将客户端注册到映射
                clients[client_socket->get_fd()] = client_socket;
                client_addresses[client_socket->get_fd()] = client_addr;

            }
            // 可读事件
            else if (ev.events & EPOLLIN) {
                handle_read_event(ev.data.fd,ep,clients, client_addresses);
            }
            else {
                printf("unknown event\n");
            }
        }
    }
    delete ep;
    delete server_socket;
    delete server_addr;
    return 0;
}
