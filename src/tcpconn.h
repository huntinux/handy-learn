#ifndef TCPCONN_H
#define TCPCONN_H

#include "channel.h"
#include "eventbase.h"
#include "net.h"

class TcpConn
{
public:
    TcpConn(EventBase* eventbase, int connfd) 
        : eventBase_(eventbase), connfd_(connfd) 
    { 
        channel_ = new Channel(eventBase_->getPoller(), connfd_, EPOLLIN);
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
        eventBase_->addChannel(channel_);
    }
    ~TcpConn() {
      if(connfd_ != kInvalidSocket)
        close(connfd_);
    }
private:
    EventBase *eventBase_;
    int connfd_;
    Channel *channel_;
};

#endif // TCPCONN_H
