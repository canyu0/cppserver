#include "util.h"
#include <cstddef>
#include <sys/epoll.h>
#include "Epoll.h"
#include <unistd.h>
Epoll::Epoll() {
    epfd = epoll_create1(0);
    errif(epfd, "epoll_create1 error");
    
}
Epoll::~Epoll() {
    close(epfd);
}
void Epoll::addFd(int fd, int events) {
    epoll_event ev;
    ev.events = events; // Set the events to monitor
    ev.data.fd = fd; // Associate the file descriptor with the event
    err1=epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(err1, "epoll_ctl error");
}
void Epoll::removeFd(int fd) {
    err1=epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
    errif(err1, "epoll_ctl error");
}
std::vector<epoll_event> Epoll::poll()
{
    const int MAX_EVENTS = 1024; // Maximum number of events to handle
    epoll_event events[MAX_EVENTS]; // Array to hold the events
    int nfds=epoll_wait(epfd, events, MAX_EVENTS, -1);
    errif(nfds < 0, "epoll_wait error");
    size_t length=nfds*sizeof(epoll_event);
    std::vector<epoll_event> result(events, events + nfds);
    return result;
}