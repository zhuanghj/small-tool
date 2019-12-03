#include<stdio.h> 
#include<sys/socket.h> 
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
 
#define MAX_LINE 1024
 
int Socket(int domain, int type, int protocol){
    int sockfd = socket(domain, type, protocol);
    if ( sockfd < 0 ){
        perror("init socket");
        exit(0);
    }
    return sockfd;
}
void Close(int clientfd){
    if ( close(clientfd) < 0){
        perror("close");
        exit(0);
    }
}
struct hostent* Gethostbyaddr(const char *addr, int len, int domain){
    struct hostent* host = gethostbyaddr(addr, len, domain);
    if ( NULL == host ){
        perror("host_by_addr");
        exit(0);
    }
    return host;
}
 
ssize_t Read(int fd, void* buf, size_t n){
    if ( read(fd, buf, n) < 0){
        perror("read");
        exit(0);
    }
}
 
ssize_t Write(int fd, const void* buf, size_t n){
    if ( write(fd, buf, n) < 0){
        perror("write");
        exit(0);
    }
}
 
void Connect(int sockfd, struct sockaddr* serv_addr, int addrlen){
    if ( connect(sockfd, serv_addr, addrlen) < 0){
        perror("connect");
        exit(0);
    }
}
 
void message_handle(int clientfd){
    size_t n;
    char send_buff[MAX_LINE];
    char recv_buff[MAX_LINE];
    memset(send_buff, 0, MAX_LINE);
    memset(recv_buff, 0, MAX_LINE);
 
    fgets(send_buff, MAX_LINE, stdin);
    send_buff[strlen(send_buff)-1] = '\0';
 
    n = Write(clientfd, send_buff, strlen(send_buff)+1);
    n = Read(clientfd, recv_buff, MAX_LINE);
 
    printf("%s \n", recv_buff);
}
 
int main(int argc, char **argv){
    int  clientfd, port;
    struct sockaddr_in servaddr;
    if ( argc != 3){
        fprintf(stderr,"usage:%s<addr> <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[2]);
    printf("port:  %d\n", port);
    printf("addr: %s\n", argv[1]);
 
    clientfd = Socket(AF_INET, SOCK_STREAM, 0);
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    Connect(clientfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    message_handle(clientfd);
    Close(clientfd);
}
--------------------- 
作者：Instant_ 
来源：CSDN 
原文：https://blog.csdn.net/sshhiixx/article/details/47377133 
版权声明：本文为博主原创文章，转载请附上博文链接！