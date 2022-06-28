#include <sys/poll.h>
#include <unistd.h>
#include "SumHandler.h"
#include "utils.h"

void SumHandler::handle(int connection_fd) {
    pollfd fds[1];
    fds[0].fd = connection_fd;
    fds[0].events = POLLIN;
    int res;

    while (true) {
        res = poll(fds, 1, -1);
        if (res < 0) {
            close(connection_fd);
            return;
        }
        long long nums = receive_num(connection_fd, 4);
        if (nums < 0) {
            close(connection_fd);
            return;
        }
        if (nums > 5) {
            send_num(connection_fd, -1, 8);
        }
        else {
            long long sum = 0, num;
            for (int i = 0; i < nums; ++i) {
                num = receive_num(connection_fd, 4);
                if (num < 0) {
                    close(connection_fd);
                    return;
                }
                sum += num;
            }
            send_num(connection_fd, sum, 8);
        }
    }
}
