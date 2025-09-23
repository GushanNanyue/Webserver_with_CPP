//
// Created by cl on 2025/9/23.
//

#include "Channel.h"

#include "Epoll.h"

Channel::Channel(Epoll *epoll, int fd)
    : epoll_(epoll), fd_(fd),relate_events_(0),return_events_(0),in_epoll_(false)
{

}

Channel::~Channel() = default;

void Channel::enable_reading() {
    relate_events_ = EPOLLIN | EPOLLET;
    epoll_->update_channel(this);
}

int Channel::get_fd() {
    return fd_;
}

uint32_t Channel::get_relate_events() {
    return relate_events_;
}

uint32_t Channel::get_return_events() {
    return return_events_;
}

bool Channel::get_in_epoll() {
    return in_epoll_;
}

bool Channel::set_in_epoll() {
    in_epoll_ = true;
}

void Channel::set_return_events(uint32_t ev) {
    return_events_ = ev;
}



