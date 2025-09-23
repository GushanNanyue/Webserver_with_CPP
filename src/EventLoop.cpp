//
// Created by cl on 2025/9/23.
//

#include "EventLoop.h"

#include "Channel.h"
#include "Epoll.h"


EventLoop::EventLoop()
    : epoll_(nullptr),quit(false)
{
    epoll_ = new Epoll(1024);
}

EventLoop::~EventLoop() {
    delete epoll_;
}

void EventLoop::loop() {
    while(!quit) {
        auto events = epoll_->wait_events(-1);
        for (auto &event : events) {
            event->handle_event();
        }
    }
}

void EventLoop::update_channel(Channel *channel) {
    epoll_->update_channel(channel);
}

Epoll* EventLoop::get_epoll() {
    return epoll_;
}


