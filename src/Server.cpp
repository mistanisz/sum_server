#include "Server.h"
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "iostream"
#include "thread"

#define MAX_CONNECTION_QUEUE 8

Server::Server(Handler *H) {
    this->handler = H;
    this->listener_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8088);
    bind(this->listener_fd, (sockaddr *)&address, sizeof(address));
}

void Server::listen() const {
    sockaddr_in address;
    int addrlen = sizeof(address);
    ::listen(this->listener_fd, MAX_CONNECTION_QUEUE);
    while (true) {
        int client_fd = accept(this->listener_fd, (sockaddr *) &address, (socklen_t *) &addrlen);
        if(client_fd < 0) std::cout << "accept error" << std::endl;
        else {
            std::thread t(&Handler::handle, this->handler, client_fd);
            t.detach();
        }
    }
}

Server::~Server() {
    close(this->listener_fd);
}
