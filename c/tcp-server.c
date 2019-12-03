
#include<stdio.h> 
#include<sys/socket.h> 
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
 
#define MAX_LISTEN 1024
#define MAX_LINE 1024
 
int Socket(int domain, int type, int protocol){
    int sockfd = socket(domain, type, protocol);
    if ( sockfd < 0 ){
        perror("init socket:  ");
        exit(0);
    }
    return sockfd;
}
 
void Bind(int sockfd, struct sockaddr *myaddr, int addrlen){
    if ( bind(sockfd, myaddr, addrlen) < 0 ){
        perror("bind");
        exit(0);
    } }
 
void Listen(int sockfd, int backlog){
    if ( listen(sockfd, backlog) < 0){
        perror("listen");
        exit(0);
    }
}
 
int Accept(int listenfd, struct sockaddr *addr, int *addrlen){
    int clientfd = accept(listenfd, addr, addrlen);
    if ( clientfd < 0){
        perror("accept");
        exit(0);
    }
    return clientfd;
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
    ssize_t num= read(fd, buf, n);
    if ( n < 0){
        perror("read");
        exit(0);
    }
    return num;
}
 
ssize_t Write(int fd, const void* buf, size_t n){
    ssize_t num = read(fd, buf, n);
    if ( n < 0){
        perror("write");
        exit(0);
    }
    return num;
}
 
void echo(listenfd){
    ssize_t n;
    char write_buff[MAX_LINE];
    char read_buff[MAX_LINE];
    
    memset(write_buff, 0, MAX_LINE);
    memset(read_buff, 0, MAX_LINE);
 
    n = read(listenfd, read_buff, MAX_LINE);
    read_buff[n] = '\0';
 
    strcpy(write_buff, "from server echo: ");
    strcpy(write_buff+strlen("from server echo: "), read_buff);
 
    n = write(listenfd, write_buff, MAX_LINE);
 
    
}
 
int main(int argc, char **argv){
    int servfd, clientfd, port, clientlen;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;
    struct hostent *host;
    char* hostaddr;
    if ( argc != 2){
        fprintf(stderr,"usage:%s<port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);  // get port
 
    servfd = Socket(AF_INET, SOCK_STREAM, 0);
    
    // init servaddr
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons((unsigned short)port);
    clientlen = sizeof(cliaddr);
    
    Bind(servfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    Listen(servfd, MAX_LISTEN);
 
    while(1){   // init server
        memset(&cliaddr, 0, sizeof(cliaddr));
        clientfd = Accept(servfd, (struct sockaddr*)&cliaddr, &clientlen);
        host = Gethostbyaddr((const char*)&cliaddr.sin_addr.s_addr, sizeof(cliaddr.sin_addr.s_addr), AF_INET);
        printf("server connect to host: %s %s\n",host->h_name, inet_ntoa(cliaddr.sin_addr));
        echo(clientfd);
        Close(clientfd);
    }
}
