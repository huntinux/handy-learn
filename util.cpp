#include "util.h"

std::string EpollEvent2String(int events)
{
    std::string evstr;
    if(events & EPOLLIN)
        evstr += "EPOLLIN ";
    if(events & EPOLLPRI)
        evstr += "EPOLLPRI";
    if(events & EPOLLOUT)
        evstr += "EPOLLOUT";
    if(events & EPOLLERR)
        evstr += "EPOLLERR";
    if(events & EPOLLHUP)
        evstr += "EPOLLHUP";
    if(events & EPOLLRDHUP)
        evstr += "EPOLLRDHUP";
    if(events & EPOLLONESHOT)
        evstr += "EPOLLONESHOT";
    if(events & EPOLLET)
        evstr += "EPOLLET";
    else
        evstr += "EPOLLLT";
    return evstr;
}
