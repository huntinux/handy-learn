#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>

/**
 * @brief The Channel class
 */
class EPoller;
class Channel
{
    using EventCallBack = std::function<void()>;
public:
    //base为事件管理器，fd为通道内部的fd，events为通道关心的事件
    Channel(EPoller* poller, int fd, int events)
        : poller_(poller),
          fd_(fd),
          events_(events)
    {

    }

    ~Channel()
    {

    }

    int fd() { return fd_; }
    short events() { return events_; }
    void close();

    void onRead(const EventCallBack& readcb) { readcb_ = readcb; }
    void onWrite(const EventCallBack& writecb) { writecb_ = writecb; }
    void onError(const EventCallBack& errorcb) { errorcb_ = errorcb; }
    void onRead(EventCallBack&& readcb) { readcb_ = std::move(readcb); }
    void onWrite(EventCallBack&& writecb) { writecb_ = std::move(writecb); }
    void onError(EventCallBack&& errorcb) { errorcb_ = std::move(errorcb); }

    void enableRead(bool enable);
    void enableWrite(bool enable);
    void enableReadWrite(bool readable, bool writable);
    bool readEnabled();
    bool writeEnabled();

    void handleRead()  { readcb_();  }
    void handleWrite() { writecb_(); }
    void handleError() { errorcb_(); }

private:
    EPoller *poller_;
    int fd_;
    short events_;
    EventCallBack readcb_, writecb_,errorcb_;
};

#endif // CHANNEL_H
