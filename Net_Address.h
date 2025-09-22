//
// Created by cl on 2025/9/22.
//

#ifndef WEBSERVER_WITH_CPP_NET_ADDRESS_H
#define WEBSERVER_WITH_CPP_NET_ADDRESS_H
#include <string_view>
#include <cstdint>
#include <netinet/in.h>

class Net_Address {
    public:
    sockaddr_in address_;
    socklen_t address_length_;
    Net_Address();
    Net_Address(std::string_view ip,uint16_t port);
    ~Net_Address();
};


#endif //WEBSERVER_WITH_CPP_NET_ADDRESS_H