//
// Created by cl on 2025/9/22.
//

#ifndef WEBSERVER_WITH_CPP_SOCKET_H
#define WEBSERVER_WITH_CPP_SOCKET_H
#include "Net_Address.h"


class Socket {
    private:
        int sock_fd_;
    public:
        Socket();
        explicit Socket(int sock_fd);
        ~Socket();

        void bind(Net_Address* addr);
        void listen();
        void set_nonblocking();

        int accept(Net_Address* addr);
        int get_fd();

};


#endif //WEBSERVER_WITH_CPP_SOCKET_H