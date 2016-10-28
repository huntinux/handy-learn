#ifndef EVENTBASE_H
#define EVENTBASE_H

#include <iostream>
#include <memory>
#include "util.h"
#include "epoller.h"


class EventBase : noncopyable
{
public:
    EventBase()
        : exited_(false),
          poller_(std::make_shared<EPoller>())
    {}
    ~EventBase() { }

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

    std::shared_ptr<EPoller> getPoller()
    {
        return poller_;
    }

private:
    bool exited_;
    std::shared_ptr<EPoller> poller_;
};


#endif // EVENTBASE_H
