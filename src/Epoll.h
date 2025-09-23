//
// Created by cl on 2025/9/22.
//

#ifndef WEBSERVER_WITH_CPP_EPOLL_H
#define WEBSERVER_WITH_CPP_EPOLL_H
#include <vector>
#include <sys/epoll.h>


class Channel;

class Epoll {
    private:
        int epoll_fd_;
        int max_events_;
        std::vector<epoll_event> events_;
    public:
        explicit Epoll(int max_events);
        ~Epoll();
        void add_event(int fd, uint32_t events);
        void remove_event(int fd);
        void update_channel(Channel* channel);

        std::vector<Channel*> wait_events(int timeout);

};


#endif //WEBSERVER_WITH_CPP_EPOLL_H