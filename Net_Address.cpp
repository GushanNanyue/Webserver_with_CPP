//
// Created by cl on 2025/9/22.
//

#include <string>
#include <arpa/inet.h>
#include "Net_Address.h"

#include <stdexcept>

Net_Address::Net_Address()
    : address_{},  // 全部清零
      address_length_{sizeof(address_)} {

}

Net_Address::Net_Address(std::string_view ip, uint16_t port)
    : address_{},  // 全部清零
      address_length_{sizeof(address_)}
{
    address_.sin_family = AF_INET;
    address_.sin_port = htons(port);
    if (inet_pton(AF_INET,ip.data(),&address_.sin_addr) <= 0) {
        throw std::invalid_argument("Invalid IP address");
    }
}

Net_Address::~Net_Address() = default;
