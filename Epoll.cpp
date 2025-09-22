//
// Created by cl on 2025/9/22.
//

#include "Epoll.h"
#include <cstdio>
#include <unistd.h>

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

std::vector<epoll_event> Epoll::wait_events(int timeout = -1) {
    int event_count = epoll_wait(epoll_fd_,events_.data(),max_events_,timeout);
    if (event_count == -1) perror("epoll_wait");
    return {events_.begin(), events_.begin() + event_count};
}


