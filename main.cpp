#include <iostream>
#include "eventbase.h"
#include "channel.h"
#include <sys/socket.h>
using namespace std;

int main()
{
    EventBase eb;
    Channel ch(eb.getPoller(), 0, EPOLLIN);
    ch.onRead([]{
        std::cout << "handle read in lambda" << std::endl;
    });
    Channel chStdout(eb.getPoller(), 1, EPOLLOUT);
    chStdout.onWrite([&]{
        std::cout << "handle write in lambda" << std::endl;
        eb.exit();
    });
    eb.addChannel(&ch);
    eb.addChannel(&chStdout);
    eb.loop();
    return 0;
}

