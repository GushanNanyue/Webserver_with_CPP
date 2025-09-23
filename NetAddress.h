//
// Created by cl on 2025/9/22.
//

#ifndef WEBSERVER_WITH_CPP_NET_ADDRESS_H
#define WEBSERVER_WITH_CPP_NET_ADDRESS_H
#include <string_view>
#include <cstdint>
#include <netinet/in.h>

class NetAddress {
    public:
    sockaddr_in address_;
    socklen_t address_length_;
    NetAddress();
    NetAddress(std::string_view ip,uint16_t port);
    ~NetAddress();
};


#endif //WEBSERVER_WITH_CPP_NET_ADDRESS_H