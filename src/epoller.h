#ifndef EPOLLER_H
#define EPOLLER_H

#include <iostream>
#include <set>
#include <sys/epoll.h>
#include <assert.h>
#include <string.h>
#include "channel.h"
#include "util.h"

/**
 * @brief The EPoller class
 * epoll in linux
 */
const int kMaxEvents = 2000;
class EPoller
{
public:
    EPoller()
        : epfd_(epoll_create1(EPOLL_CLOEXEC)), quit_(false)
    {
        if(epfd_ < 0)
        {
            std::cout << "FATAL ERROR: epoll_create1 failed" << std::endl;
            assert(0);
        }
    }
    ~EPoller(){}

    void loop() {
      while(!quit_) {
        loop_once();
      }
    }

    void exit() {
      quit_ = true;
    }

    void loop_once() {
        std::cout << "EVENT LOOP STARTS" << std::endl;
        int n = epoll_wait(epfd_, activeEvs_, kMaxEvents, 10000);
        for(int i = 0; i < n; i++) {
            Channel* ch = (Channel*)activeEvs_[i].data.ptr;
            int events = activeEvs_[i].events;
            if (ch) {
                if (events & EPOLLIN ) {
                    std::cout <<"channel:" << std::hex << ch
                              <<" fd:" << ch->fd()
                              <<" handle read" << std::endl;
                    ch->handleRead();
                } else if (events & EPOLLOUT) {
                    std::cout <<"channel:" << std::hex << ch
                              <<" fd:" << ch->fd()
                              <<" handle write" << std::endl;
                    ch->handleWrite();
                } else { // EPOLLERR EPOLLHUP
                    std::cout <<"channel:" << std::hex << ch
                              <<" fd:" << ch->fd()
                              <<" handle error" << std::endl;
                    ch->handleError();
                }
            }
        }
    }
    void addChannel(Channel* ch)
    {
        struct epoll_event ev;
        memset(&ev, 0, sizeof(ev));
        ev.events = ch->events();
        ev.data.ptr = ch;
        std::cout <<"add channel:" << std::hex << ch
                  <<" fd:" << ch->fd()
                  <<" events:" << EpollEvent2String(ch->events())
                  <<" epoll:"<<epfd_ << std::endl;
        int r = epoll_ctl(epfd_, EPOLL_CTL_ADD, ch->fd(), &ev);
        if(r < 0)
        {
            std::cout << "epoll_ctl add failed errno:" << errno
                      << " :" << strerror(errno) << std::endl;
            assert(0);
        }
        channels_.insert(ch);
    }

    void removeChannel(Channel* ch);
    void updateChannel(Channel* ch);
    void loop_once(int waitMs);
private:
    int epfd_;
    std::set<Channel*> channels_;
    struct epoll_event activeEvs_[kMaxEvents];
    bool quit_;
};

#endif // EPOLLER_H
