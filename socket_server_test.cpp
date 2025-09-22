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

// void set_nonblocking(int fd) {
//     int flag = fcntl(fd,F_GETFL,0);
//     if (flag == -1) {
//         perror("fcntl failed");
//     }
//     if (fcntl(fd,F_SETFL,flag | O_NONBLOCK) == -1)
//         perror("fcntl failed");
// }
//
// int main() {
//     sockaddr_in server_addr{};
//
//     // 老三样 create-bind-listen
//     int server_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_fd < 0) {
//         std::cerr << "Socket creation failed." << std::endl;
//         return -1;
//     }
//
//     // 设置地址重用，防止地址占用
//     int opt = 1;
//     setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
//
//     // 配置server地址：协议、host、port
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(8000);
//
//     // bind
//     if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&server_addr),sizeof(server_addr))<0) {
//         std::cerr << "Bind failed." << std::endl;
//         close(server_fd);
//         return -1;
//     }
//
//     // listen
//     if (listen(server_fd, 1) < 0) {
//         std::cerr << "Listen failed." << std::endl;
//         close(server_fd);
//         return -1;
//     }
//
//     // 将服务端设置成非阻塞
//     set_nonblocking(server_fd);
//
//     // 创建epoll
//     int epoll_fd = epoll_create1(0);
//     if (epoll_fd < 0) {
//         std::cerr << "epoll_create failed." << std::endl;
//         close(server_fd);
//         close(epoll_fd);
//         return -1;
//     }
//
//     // 将监听 fd 加入epoll
//     epoll_event ev{};
//     ev.events = EPOLLIN;
//     ev.data.fd = server_fd;
//     if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) < 0) {
//         std::cerr << "epoll_ctl failed." << std::endl;
//         close(server_fd);
//         close(epoll_fd);
//         return -1;
//     }
//
//     // 创建返回事件数组
//     std::vector<epoll_event> events(MAX_EVENTS);
//
//     // 持续监听
//     while (true) {
//         // 获取事件队列长度
//         int event_count = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
//         if (event_count == -1) {
//             if (errno == EINTR) {
//                 continue;
//             }
//             std::cerr << "epoll_wait failed." << std::endl;
//             break;
//         }
//         // 遍历事件队列进行处理
//         for (int i = 0; i < event_count; i++) {
//             // 如果触发事件的文件描述符是被监听的server_fd代表有新连接
//             if (events[i].data.fd == server_fd) {
//                 // 循环接受连接
//                 while (true) {
//                     sockaddr_in client_addr{};
//                     socklen_t client_addr_len = sizeof(client_addr);
//                     int conn_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr), &client_addr_len);
//                     if (conn_fd < 0) {
//                         if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                             break;
//                         }
//                         std::cerr << "accept failed." << std::endl;
//                         break;
//                     }
//
//                     // 将客户端文件描述符加入监听队列
//                     epoll_event client_event{};
//                     client_event.data.fd = conn_fd;
//                     client_event.events = EPOLLIN | EPOLLET;
//                     set_nonblocking(conn_fd);
//                     if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &client_event) < 0) {
//                         perror("epoll_ctl: conn_fd");
//                         close(conn_fd);
//                         continue;
//                     }
//                 }
//             }
//             // 如果是客户端可读事件
//             else if (events[i].events & EPOLLIN) {
//                 while (true) {
//                     char buf[READ_BUFFER_SIZE];
//                     size_t read_size = recv(events[i].data.fd, buf, READ_BUFFER_SIZE,0);
//                     // 收到数据
//                     if (read_size > 0) {
//                         printf("message from client fd %d: %s\n", events[i].data.fd, buf);
//                         // 返回HTTP 数据
//                         auto write_len = strlen(HTTP_RESPONSE);
//                         const char *write_buf = HTTP_RESPONSE;
//                         while (write_len > 0) {
//                             int w = send(events[i].data.fd, write_buf, write_len, 0);
//                             if (w < 0) {
//                                 // 客户端套接字不能写
//                                 if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                                     //TODO 后续应添加处理
//                                     break;
//                                 }
//                                 perror("send failed");
//                                 break;
//                                 }
//                             write_len -= w;
//                             write_buf += w;
//                         }
//                     }
//                     else if (read_size < 0) {
//                         if (errno != EAGAIN && errno != EWOULDBLOCK) {
//                             perror("recv");
//                             epoll_ctl(epoll_fd,EPOLL_CTL_DEL, events[i].data.fd, nullptr);
//                             close(events[i].data.fd);
//                         }
//                         // 如果是 EAGAIN，说明没有数据，下一轮会再通知
//                         break;
//                     }
//                     else if (read_size == 0) {
//                         printf("client fd %d connection closed\n",events[i].data.fd);
//                         epoll_ctl(epoll_fd,EPOLL_CTL_DEL, events[i].data.fd, nullptr);
//                         close(events[i].data.fd);
//                         break;
//                     }
//                 }
//             }
//             // TODO 其他事件后续再定义
//             else {
//                 printf("something bad happened.\n");
//             }
//         }
//     }
//     close(epoll_fd);
//     close(server_fd);
//     return 0;
// }

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
