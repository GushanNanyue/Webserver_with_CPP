//
// Created by cl on 2025/9/22.
//

#include "Epoll.h"
#include <cstdio>
#include <unistd.h>

#include "Channel.h"


Epoll::Epoll(int max_events)
    :max_events_(max_events)
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        perror("epoll_create1");
    }

    events_.resize(max_events);
}

Epoll::~Epoll() {
    if (epoll_fd_ > 0) {
        close(epoll_fd_);
    }
}

void Epoll::add_event(int fd, uint32_t events) {
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD,fd, &ev) < 0) {
        perror("epoll_ctl add");
    }

}

void Epoll::remove_event(int fd) {
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
        perror("epoll_ctl delete");
    }
}

void Epoll::update_channel(Channel* channel) {
    int fd = channel->get_fd();
    epoll_event ev{};
    ev.data.ptr = channel;
    ev.events = channel->get_relate_events();
    if (!channel->get_in_epoll()) {
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) < 0) {
            perror("epoll_ctl add");
        }
        channel->set_in_epoll();
    }else {
        if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev) < 0) {
            perror("epoll_ctl modify");
        }
    }
}


std::vector<Channel*> Epoll::wait_events(int timeout = -1) {
    int event_count = epoll_wait(epoll_fd_,events_.data(),max_events_,timeout);
    if (event_count == -1) perror("epoll_wait");
    std::vector<Channel*> active_channels;
    for (int i = 0; i < event_count; i++) {
        auto *ch = static_cast<Channel *>(events_[i].data.ptr);
        ch->set_return_events(events_[i].events);
        active_channels.push_back(ch);
    }
    return active_channels;
}


