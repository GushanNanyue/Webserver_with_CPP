//
// Created by cl on 2025/9/23.
//

#ifndef WEBSERVER_WITH_CPP_EVENTLOOP_H
#define WEBSERVER_WITH_CPP_EVENTLOOP_H


class Epoll;
class Channel;
class EventLoop {
    private:
        Epoll* epoll_;
        bool quit;
    public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update_channel(Channel* channel);
        Epoll* get_epoll();

};


#endif //WEBSERVER_WITH_CPP_EVENTLOOP_H