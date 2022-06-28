#ifndef SUM_SERVER_SUMHANDLER_H
#define SUM_SERVER_SUMHANDLER_H

#include "Handler.h"

class SumHandler: public Handler {
public:
    void handle(int) override;
};


#endif //SUM_SERVER_SUMHANDLER_H
