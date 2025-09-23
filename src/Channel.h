//
// Created by cl on 2025/9/23.
//

#ifndef WEBSERVER_WITH_CPP_CHANNEL_H
#define WEBSERVER_WITH_CPP_CHANNEL_H
#include <functional>
#include <sys/epoll.h>




class EventLoop;
class Channel {
    private:
        EventLoop *loop_;
        int fd_;
        uint32_t relate_events_;
        uint32_t return_events_;
        bool in_epoll_;
        std::function<void()> callback_;
    public:
        Channel(EventLoop* loop, int fd);
        ~Channel();

        void enable_reading();
        void handle_event();

        int get_fd();
        uint32_t get_relate_events();
        uint32_t get_return_events();
        bool get_in_epoll();
        void set_in_epoll(bool);

        void set_return_events(uint32_t);
        void set_callback(std::function<void()>);

};


#endif //WEBSERVER_WITH_CPP_CHANNEL_H