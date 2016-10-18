#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <sys/epoll.h>

/**
 * @brief make class non-copyable in c++11
 * same as boost::noncopyable
 */
struct noncopyable {
    noncopyable(){}
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

/**
 * @brief EpollEvent2String
 * return epoll event type in string
 * @return
 */
std::string EpollEvent2String(int events);

/**
 * @brief kInvalidSocket
 */
extern const int kInvalidSocket;

/**
 * @brief create_and_bind
 * @param addr server's address
 * @param port server's port
 * @return socket fd, -1 for invalid socket
 */
//int create_and_bind(const char* addr, short port);

#endif // UTIL_H

