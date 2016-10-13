#ifndef EVENTBASE_H
#define EVENTBASE_H

#include <iostream>
#include "util.h"
#include "epoller.h"


class EventBase : noncopyable
{
public:
    EventBase()
        : exited_(false), poller_(new EPoller)
    {}
    ~EventBase() { delete poller_; }

    void exit()
    {
        exited_ = true;
    }

    void loop()
    {
        while(!exited_)
            poller_->loop();
    }

    void addChannel(Channel* ch)
    {
        poller_->addChannel(ch);
    }

    EPoller* getPoller()
    {
        return poller_;
    }

private:
    bool exited_;
    EPoller* poller_;
};


#endif // EVENTBASE_H
