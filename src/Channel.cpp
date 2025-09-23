//
// Created by cl on 2025/9/23.
//

#include "Channel.h"

#include <utility>

#include "EventLoop.h"


Channel::Channel(EventLoop *loop, int fd)
    : loop_(loop), fd_(fd),relate_events_(0),return_events_(0),in_epoll_(false)
{

}

Channel::~Channel() = default;

void Channel::enable_reading() {
    relate_events_ = EPOLLIN | EPOLLET;
    loop_->update_channel(this);
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

void Channel::set_in_epoll(bool state) {
    in_epoll_ = state;
}

void Channel::set_return_events(uint32_t ev) {
    return_events_ = ev;
}

void Channel::set_callback(std::function<void()> call_back_function) {
    callback_ = std::move(call_back_function);
}

void Channel::handle_event() {
    callback_();
}



