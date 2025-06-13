#include <sys/socket.h>
#include <arpa/inet.h>  //这个头文件包含了<netinet/in.h>，不用再次包含了
#include<cstring>
#include<cstdio>
#include <cstdlib>
#include <unistd.h>
struct sockaddr_in serv_addr;
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
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));
    char buf[1024];
    while(1)
    {
        
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes=read(clnt_sockfd, buf, sizeof(buf));
        if(read_bytes <0) {
            perror("recv error");
            close(clnt_sockfd);
            break;
        }
        else if(read_bytes == 0) {
            printf("Client fd %d disconnected.\n", clnt_sockfd);
            close(clnt_sockfd);
            break;
        }
        else{
            write(clnt_sockfd, buf, read_bytes); // Echo back
            printf("Received: %s\n", buf);
        }
        
        
    }
    return 0;
}