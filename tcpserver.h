#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <string>
#include <functional>
#include <map>
#include <iostream> // TODO: Change to log util
#include "util.h"
#include "net.h"
#include "channel.h"
#include "eventbase.h"
#include "tcpconn.h"

/**
 * @brief The TcpServer class
 * Only support IPV4
 * Default address:port is 0.0.0.0:8080
 */
class TcpServer : private noncopyable
{
    using EventHandler = std::function<void()>;
public:

    TcpServer(EventBase *eventBase,
              const std::string port = "8080",
              const std::string addr = "0.0.0.0")
        : eventBase_(eventBase),
          port_(port),
          addr_(addr),
          listenfd_(kInvalidSocket),
          listenChannel_(nullptr)
    {
        start();
    }

    ~TcpServer() {
        delete listenChannel_;
    }

    void HandleAccept()
    {
        struct sockaddr_in sa;
        socklen_t sa_len = sizeof(struct sockaddr_in);
        int connfd = accept(listenfd_, (struct sockaddr *)&sa, &sa_len);
        assert(connfd != kInvalidSocket);
        printf_address(connfd, (struct sockaddr *)&sa, sa_len, "Accept");
      
        /* create new TcpConn to handle the connection */
        TcpConn* conn = new TcpConn(eventBase_, connfd);
        connections_[connfd] = conn;
    }

private:
    bool start()
    {
        listenfd_ = create_and_bind(port_.c_str(), addr_.c_str());
        assert(listenfd_ != kInvalidSocket);
        assert(listen(listenfd_, SOMAXCONN) != -1);
        listenChannel_ = new Channel(eventBase_->getPoller(), listenfd_, EPOLLIN);
        listenChannel_->onRead([this]{
            std::cout << "Handle Read In TcpServer." << std::endl;
            HandleAccept();
        });
        listenChannel_->onWrite([]{
            std::cout << "Handle Write In TcpServer." << std::endl;
        });
        listenChannel_->onError([]{
            std::cout << "Handle Error In TcpServer." << std::endl;
        });
        eventBase_->addChannel(listenChannel_);
    }

private:
    EventBase *eventBase_;
    std::string port_;
    std::string addr_;
    int listenfd_;
    Channel *listenChannel_;
    EventHandler readcb_, writecb_, errorcb_;
    std::map<int, TcpConn*> connections_; // one connected fd one TcpConn
};

#endif // TCPSERVER_H
