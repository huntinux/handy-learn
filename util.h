#ifndef UTIL_H
#define UTIL_H

/**
 * @brief make class non-copyable in c++11
 * same as boost::noncopyable
 */
struct noncopyable {
    noncopyable(){}
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

#endif // UTIL_H

