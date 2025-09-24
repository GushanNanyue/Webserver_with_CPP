//
// Created by cl on 2025/9/24.
//

#include "Acceptor.h"

#include <utility>

#include "Channel.h"
#include "Socket.h"


Acceptor::Acceptor(EventLoop *loop)
    :loop_(loop)
{
    // “老三样”
    // 创建 server socket
    socket_ = new Socket();
    // 创建 server addr
    address_ = new NetAddress("127.0.0.1",8000);
    // bind
    socket_->bind(address_);
    // listen
    socket_->listen();
    socket_->set_nonblocking();

    // 创建 channel 关联 epoll 和 fd
    accept_channel_ = new Channel(loop,socket_->get_fd());

    std::function cb = [this] { accept_connection(); };
    accept_channel_->set_callback(cb);
    accept_channel_->enable_reading();
}

Acceptor::~Acceptor() {
    delete socket_;
    delete address_;
    delete accept_channel_;
}

void Acceptor::accept_connection() {
    accept_cb(socket_);
}

void Acceptor::set_new_connection_callback(std::function<void(Socket*)> cb) {
    accept_cb = std::move(cb);
}

