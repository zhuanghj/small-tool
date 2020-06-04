#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include <tcp.h>
#include <pthread.h>

static int32_t tc_Init()
{
    int32_t sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        tcpDbg("client socket error");
        return 0;
    }
    return sockfd;
}
static int32_t tc_Connect(uint8_t ip[IP_MAX_LEN], uint16_t port)
{
    struct sockaddr_in servaddr;
    int8_t ret = 0;
    int32_t sockfd = 0;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        tcpDbg("client socket error");
        return 0;
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    ret = connect(sockfd, &servaddr, sizeof(servaddr));
    if (ret < 0)
    {
        tcpDbg("connect to server %s:%d fail ret = %d", ip, port, ret);
        return ret;
    }
    return sockfd;
}
static int32_t tc_Read(int32_t fd, uint8_t *buff, uint32_t size)
{
    int32_t ret = 0;
    if (fd < 0 || buff == NULL)
    {
        tcpDbg("input param error");
        return -1;
    }
    ret = read(fd, buff, size);
    return ret;
}
static int32_t tc_Wirte(int32_t fd, uint8_t *buff, uint32_t size)
{
    int32_t ret = 0;
    if (fd < 0 || buff == NULL)
    {
        tcpDbg("input param error");
        return -1;
    }
    ret = write(fd, buff, size);
    return ret;
}

static int8_t tc_Disconnect(int32_t fd)
{
    if (fd < 0)
    {
        tcpDbg("input param error");
        return -1;
    }
    return close(fd);
}
static void *thread_start(void *arg)
{
    struct timeval time;
    fd_set set;
    struct tcpParam *param = NULL;
    if (NULL == arg)
    {
        tcpDbg("input param error");
        return NULL;
    }
    param = arg;
    while ((param->isConnect) && (param->fd > 0))
    {
        time.tv_sec = 0;
        time.tv_usec = 50000; //50ms

        FD_ZERO(&set);
        FD_SET(param->fd, &set);
        ret = select(param->fd + 1, &set, NULL, NULL, &set);
        if (ret == 0)
            continue;
        else if (ret < 0)
        {
            tcpDbg("client select error socket disconnected by server");
            param->isConnect = 0;
            tc_Disconnect(param->fd);
            param->fd = 0;
            continue;
        }
        else if (ret > 0)
        {
            len = tc_Read(param->fd, param->recvData, readlen);
            if (len < 0)
                tcpDbg("tcp read error len = %d", len);
            else if (len == 0)
            {
                tcpDbg("client socket disconnect %x", tcpInfo);
                param->isConnect = 0;
                tc_Disconnect(tcpInfo->fd);
                param->fd = 0;
            }
            continue;
        }
    }
    return NULL;
}

int8_t tcpClientInit(void *tParam)
{
    int8_t ret = 0;
    int32_t fd = 0;
    struct tcpParam *param = NULL;
    if (NULL == tParam)
    {
        tcpDbg("input param error");
        return -1;
    }
    param = tParam;
    fd = tc_Init();
    if (fd < 0)
    {
        tcpDbg("tc_Init error");
        return -2;
    }
    ret = tc_Connect(param->ip, param->port);
    if (ret < 0)
    {
        tcpDbg("connect fail");
        return -3;
    }

    /*pthread*/
    if (param->fd > 0)
        pthread_create(&pId, NULL, thread_start, param);
    else
    {
        tcpDbg("fd error we cant read or write through this\n");
        return -4;
    }
}

int32_t tcpClientRead(struct tcpParam *param, uint8_t *buff, int32_t size)
{
    int32_t cpSize = 0;
    if (buff == NULL || param == NULL)
    {
        tcpDbg("input param error");
        return -1;
    }
    cpSize = param->recvLen > size ? size : param->recvLen;
    /*lock*/
    memcpy(buff, param->recvData, cpSize);
    /*unlock*/
    return cpSize;
}

int8_t tcpClientWrite(struct tcpParam *param, uint8_t *buff, int32_t size)
{
}
