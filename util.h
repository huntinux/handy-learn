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

#endif // UTIL_H

