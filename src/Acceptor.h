//
// Created by cl on 2025/9/24.
//

#ifndef WEBSERVER_WITH_CPP_ACCEPTOR_H
#define WEBSERVER_WITH_CPP_ACCEPTOR_H
#include <functional>


class Channel;
class NetAddress;
class Socket;
class EventLoop;

class Acceptor {
    private:
        EventLoop *loop_;
        Socket * socket_;
        NetAddress* address_;
        Channel * accept_channel_;

    public:
        std::function<void (Socket *)> accept_cb;

    public:
        explicit Acceptor(EventLoop *loop);
        ~Acceptor();
        void accept_connection();
        void set_new_connection_callback(std::function<void(Socket*)>);
};


#endif //WEBSERVER_WITH_CPP_ACCEPTOR_H