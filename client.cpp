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
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    bzero(&clnt_addr, sizeof(clnt_addr));
    int err=connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr));
    errif(err, "listen error");
    char buf[1024];
    while(1)
    {
        bzero(&buf, sizeof(buf));
        scanf("%s", buf); // Read input from user
        ssize_t write_bytes = write(sockfd, buf, strlen(buf));
        if(write_bytes <0) {
            printf("cannot write to server\n");
            break;
        }
        bzero(&buf, sizeof(buf));
        ssize_t read_bytes = read(sockfd, buf, sizeof(buf));
        if(read_bytes < 0) {
            close(sockfd);
            perror("recv error");
            break;
        }
        else if(read_bytes == 0) {
            printf("Server disconnected.\n");
            break;
        }
        else{
            printf("Received from server: %s\n", buf);
        }
        
        
    }
    return 0;
}