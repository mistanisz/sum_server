#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "utils.h"

long long receive_num(int connection_fd, long bytes_to_recv) {
    long long value=0;
    char* recv_buffer = (char*)&value;
    while (bytes_to_recv > 0) {
        ssize_t read_bytes = read(connection_fd, recv_buffer, bytes_to_recv);
        if (read_bytes <= 0) {
            return -1;
        }
        bytes_to_recv -= read_bytes;
    }
    return value;
}

void send_num(int connection_fd, long long num, long bytes_to_send){
    char* buffer = (char*)&num;
    while(bytes_to_send > 0) {
        ssize_t r = send(connection_fd, buffer, bytes_to_send, 0);
        if (r <= 0) {
            std::cout << "error sending num" << std::endl;
            return;
        }
        bytes_to_send-=r;
    }
}
