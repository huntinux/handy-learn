#include <iostream>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <unistd.h>

#include "channel.h"
#include "tcpserver.h"

using namespace std;

int main()
{
#define TEST_TCPSERVER
#ifdef TEST_TCPSERVER
    /**
     * Test TcpServer, TcpConn
     */
    TcpServer server("9999");
    server.start();
#endif


    /**
     * Test Channel, Poller
     */
//#define TEST_TIMERFD
#ifdef TEST_TIMERFD
    EPoller epoller;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(timerfd >= 0);
    Channel channel(&epoller, timerfd, EPOLLIN);
    channel.onRead([&]{
        std::cout << "Timeout" << std::endl;
        epoller.exit();
    });
    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 2;
    int r = ::timerfd_settime(timerfd, 0, &howlong, NULL);
    if(r == -1) {
        perror("timerfd_settime");
        exit(EXIT_FAILURE);
    }
    epoller.addChannel(&channel);
    epoller.loop();
    ::close(timerfd);
#endif
    return 0;
}

