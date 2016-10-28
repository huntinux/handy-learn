#ifndef TCPCONN_H
#define TCPCONN_H

#include "channel.h"
#include "util.h"
#include "net.h"
#include "epoller.h"

class TcpConn
{
public:
    TcpConn(EPoller* epoller, int connfd) 
        : epoller_(epoller), connfd_(connfd) 
    { 
        channel_ = new Channel(epoller_, connfd_, EPOLLIN);
        channel_->onRead([this]{
            std::cout << "Handle Read In TcpServer." << std::endl;
            close(connfd_);
            connfd_ = kInvalidSocket;
        });
        channel_->onWrite([]{
            std::cout << "Handle Write In TcpServer." << std::endl;
        });
        channel_->onError([]{
            std::cout << "Handle Error In TcpServer." << std::endl;
        });
        epoller_->addChannel(channel_);
    }
    ~TcpConn() {
      if(connfd_ != kInvalidSocket)
        close(connfd_);
    }
private:
    EPoller* epoller_;
    int connfd_;
    Channel *channel_;
};

#endif // TCPCONN_H
