#ifndef SUM_SERVER_SERVER_H
#define SUM_SERVER_SERVER_H


#include "Handler.h"

class Server {
public:
    explicit Server(Handler*);
    ~Server();

    void listen() const;
private:
    int listener_fd;
    Handler *handler;
};


#endif //SUM_SERVER_SERVER_H
