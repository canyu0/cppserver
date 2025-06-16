#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll{
    public:
        int epfd; // File descriptor for the epoll instance

        Epoll();
        ~Epoll();
        void addFd(int fd, int events);
        void removeFd(int fd);
        std::vector<epoll_event> poll();

};