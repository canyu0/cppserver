#include <cerrno>
#include <sys/socket.h>
#include <arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/epoll.h>
#include<fcntl.h>

void setnonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl get flags error");
        exit(EXIT_FAILURE);
    }
    flags |= O_NONBLOCK; // Set the O_NONBLOCK flag
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("fcntl set non-blocking error");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in serv_addr;
const int MAX_EVENTS = 10; // Maximum number of events to handle
int sockfd = socket(AF_INET, SOCK_STREAM, 0);
void errif(bool condition, const char *errmsg){
    if(condition){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
void errif(int condition, const char *errmsg){
    if(condition<0){
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}
int main()
{
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);                               

    int err=bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(err, "bind error");    
    err=listen(sockfd, SOMAXCONN);
    errif(err, "listen error");

    int epfd=epoll_create1(0);
    errif(epfd, "epoll_create1 error");
    struct epoll_event events[MAX_EVENTS];
    struct epoll_event ev;
    ev.events = EPOLLIN; // Monitor for input events
    ev.data.fd = sockfd; // Monitor the server socket
    err = epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);
    errif(err, "epoll_ctl error");
    while(1)
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds<0, "epoll_wait error");
        for(int i=0;i<nfds;i++)
        {
            if(events[i].data.fd==sockfd)
            {
                struct sockaddr_in clnt_addr;
                socklen_t clnt_addr_len = sizeof(clnt_addr);
                bzero(&clnt_addr, sizeof(clnt_addr));
                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                errif(clnt_sockfd < 0, "accept error");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
                ev.data.fd = clnt_sockfd; // Add the new client socket to epoll
                ev.events = EPOLLIN | EPOLLET; // Monitor for input events
                setnonblocking(clnt_sockfd); // Set the client socket to non-blocking mode
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);

            }
            else if(events[i].events & EPOLLIN) // Check if the event is for input
            {
                int clnt_sockfd = events[i].data.fd;
                char buf[1024];
                while(1)
                {
                    bzero(&buf, sizeof(buf));
                    ssize_t read_bytes = read(clnt_sockfd, buf, sizeof(buf));
                    if(read_bytes < 0 && errno==EINTR) {
                        continue;
                    }
                    else if(read_bytes < 0 && (errno==EAGAIN || errno==EWOULDBLOCK)) {
                        // No more data to read, continue to next event
                        break;
                    }
                    else if(read_bytes == 0) {
                        printf("Client fd %d disconnected.\n", clnt_sockfd);
                        close(clnt_sockfd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, clnt_sockfd, NULL); // Remove from epoll
                        break;
                    }
                    else{
                        write(clnt_sockfd, buf, read_bytes); // Echo back
                        printf("Received: %s\n", buf);
                    }
                }
            }
            else{

            }

        }

    }
    
    return 0;
}