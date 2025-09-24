//
// Created by cl on 2025/9/23.
//

#ifndef WEBSERVER_WITH_CPP_SERVER_H
#define WEBSERVER_WITH_CPP_SERVER_H

class Epoll;
class Socket;
class EventLoop;
class Acceptor;
class Server {
    private:
        EventLoop *loop_;
        Acceptor* acceptor_;
    public:
        explicit Server(EventLoop *loop);
        ~Server();

        void handle_read_event(int fd ,Epoll* epoll);
        void handle_new_connection(Socket * socket);
};


#endif //WEBSERVER_WITH_CPP_SERVER_H