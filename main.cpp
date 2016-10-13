#include <iostream>
#include "eventbase.h"
#include "channel.h"
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <unistd.h>

using namespace std;

int main()
{
    EventBase eb;
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    assert(timerfd >= 0);
    Channel channel(eb.getPoller(), timerfd, EPOLLIN);
    channel.onRead([&]{
        std::cout << "Timeout" << std::endl;
        eb.exit();
    });
    struct itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 2;
    int r = ::timerfd_settime(timerfd, 0, &howlong, NULL);
    if(r == -1) {
        perror("timerfd_settime");
        exit(EXIT_FAILURE);
    }
    eb.addChannel(&channel);
    eb.loop();
    ::close(timerfd);
    return 0;
}

