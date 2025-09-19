#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>



int main() {
    sockaddr_in server_addr{}, client_addr{};
    int client_addr_len = sizeof(client_addr);

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        std::cerr << "Socket creation failed." << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);

    if (bind(server_fd, reinterpret_cast<struct sockaddr *>(&server_addr),sizeof(server_addr))<0) {
        std::cerr << "Bind failed." << std::endl;
        return -1;
    }

    if (listen(server_fd, 1) < 0) {
        std::cerr << "Listen failed." << std::endl;
        return -1;
    }
    while (true) {
        int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr *>(&client_addr),
                           reinterpret_cast<socklen_t *>(&client_addr_len));
        if (client_fd < 0) {
            std::cerr << "Connection failed." << std::endl;
            return -1;
        }

        char buffer[1024];
        recv(client_fd, buffer, 1024, 0);
        std::cout << "client send message:" << buffer << std::endl;


        std::string http_response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html; charset=UTF-8\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<h1> Hello World </h1>";

        send(client_fd, http_response.c_str(), http_response.size(), 0);

        close(client_fd);
    }
    close(server_fd);
    return 0;
}
