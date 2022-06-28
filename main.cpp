#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include <iostream>
#include <unistd.h>
#include <random>
#include "src/utils.h"
#include "src/Handler.h"
#include "src/SumHandler.h"
#include "src/Server.h"

#define PORT 8088
#define SERVER_ADDR "127.0.0.1"

Server* setup() {
    Handler *H = new SumHandler();
    auto *S = new Server(H);
    std::thread t(&Server::listen, S);
    t.detach();
    return S;
}

int start_client(){
    sockaddr_in address;
    int connection_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_ADDR, &address.sin_addr);
    connect(connection_fd, (struct sockaddr *)&address, sizeof(address));
    return connection_fd;
}

bool single_value_test(){
    std::cout << __FUNCTION__;
    int conn = start_client();
    send_num(conn, 1, 4);
    send_num(conn, 8, 4);
    long long res = receive_num(conn, 8);
    close(conn);
    return res == 8;
}

bool multi_value_test_internal() {
    int conn = start_client();
    send_num(conn, 5, 4);
    for(int i=0;i<5;++i)
        send_num(conn, i, 4);
    long long res = receive_num(conn, 8);
    close(conn);
    return res == 10;
}

bool multi_value_test(){
    std::cout << __FUNCTION__ << " ";
    return multi_value_test_internal();
}

bool over_limit_test() {
    std::cout << __FUNCTION__;
    int conn = start_client();
    send_num(conn, 6, 4);
    long long res = receive_num(conn, 8);
    close(conn);
    return res == -1;
}

bool max_values_test() {
    std::cout << __FUNCTION__;
    int conn = start_client();
    send_num(conn, 5, 4);
    for(int i=0;i<5;++i)
        send_num(conn, 4294967295, 4);
    long long res = receive_num(conn, 8);
    close(conn);
    return res == 21474836475;
}

void multiclient_test_helper(int* res){
    *res = multi_value_test_internal();
}

bool multiclient_test() {
    std::cout << __FUNCTION__;
    int res[32] = {0};
    std::vector<std::thread> threads;
    for(int i=0;i<32;++i) {
        threads.emplace_back(&multiclient_test_helper, res+i);
    }
    for(auto& t: threads) {
        if(t.joinable()) t.join();
    }
    bool final_res = true;
    for(auto r: res) final_res = final_res && r;
    return final_res;
}

bool multirequest_test() {
    std::cout << __FUNCTION__;
    int conn = start_client();
    bool final_res = true;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> uniform_dist(0, INT32_MAX);
    for(int i=0;i<32;++i){
        send_num(conn, 5, 4);
        long long expected_sum = 0;
        for(int j=0;j<5;++j) {
            int num = uniform_dist(gen);
            send_num(conn, num, 4);
            expected_sum += num;
        }
        long long res = receive_num(conn, 8);
        final_res = final_res && (res == expected_sum);
    }
    close(conn);
    return final_res;
}

int main() {
    Server* S = setup();
    bool (*tests [6])() = {
            &single_value_test,
            &multi_value_test,
            &over_limit_test,
            &max_values_test,
            &multiclient_test,
            &multirequest_test
    };
    for(auto test: tests){
        if(test()) std::cout << " \tPASSED" << std::endl;
        else std::cout << " \tFAILED" << std::endl;
    }
    delete S;
    return 0;
}
