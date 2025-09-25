//
// Created by cl on 2025/9/24.
//

#ifndef WEBSERVER_WITH_CPP_CONNECTION_H
#define WEBSERVER_WITH_CPP_CONNECTION_H
#include <functional>


class Channel;
class EventLoop;
class Socket;

class Connection {
    private:
        Socket * socket_;
        EventLoop * loop_;
        Channel * channel_;
        std::function<void(Socket*)> delete_connection_callback_;
    public:
        Connection(EventLoop* ,Socket* );
        ~Connection();

        void handle_read_event(int);
        void set_delete_connection_callback(std::function<void(Socket*)>);

};


#endif //WEBSERVER_WITH_CPP_CONNECTION_H