#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include "tcp.h"
#include <pthread.h>
#include <errno.h>

static int32_t tc_Init(struct tcpParam *param)
{
    int32_t sockfd = 0;
    param->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (param->fd < 0)
    {
        tcpDbg("client socket error");
        return 0;
    }
    return param->fd;
}
static int32_t tc_Connect(struct tcpParam *param)
{
    struct sockaddr_in servaddr;
    int8_t ret = 0;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(param->port);
    inet_pton(AF_INET, param->ip, &servaddr.sin_addr);
    ret = connect(param->fd, &servaddr, sizeof(servaddr));
    if (ret < 0)
    {
        tcpDbg("connect to server %s:%d fail ret = %d", param->ip, param->port, ret);
        return ret;
    }
    return ret;
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
static int32_t tc_Write(int32_t fd, uint8_t *buff, uint32_t size)
{
    int32_t ret = 0;
    if ((fd < 0) || (buff == NULL))
    {
        tcpDbg("input param error");
        return -1;
    }
    if(size != 0)
    {
	    ret = write(fd, buff, size);
    }
    return ret;
}

static int8_t tc_Disconnect(int32_t fd)
{
    if (fd < 0)
    {
        tcpDbg("input param error");
        return -1;
    }
    tcpDbg("socket fd %d close\n",fd);
    return close(fd);
}
static void *thread_start(void *arg)
{
    struct timeval time;
    fd_set set;
    uint32_t len = 0;
    int8_t ret = 0;
    struct tcpParam *param = NULL;
    if (NULL == arg)
    {
        tcpDbg("input param error");
        return NULL;
    }
	pthread_detach(pthread_self());
    param = arg;
    while ((param->isConnect) && (param->fd > 0))
    {
		sleep(1);
        time.tv_sec = 0;
        time.tv_usec = 50000; //50ms
		
		tcpDbg("send data : %s len = %d\n",param->sendData,param->sendLen);
        ret = pthread_mutex_trylock(&(param->sendMutex));
        while (ret != 0)
        {
            ret = pthread_mutex_trylock(&(param->sendMutex));
            usleep(100);
        }
        ret = tc_Write(param->fd, param->sendData, param->sendLen);
		memset(param->sendData,0,sizeof(param->sendData));
		param->sendLen = 0;
        pthread_mutex_unlock(&(param->sendMutex));
        if (ret < 0)
        {
            tcpDbg("write error");
            return -1;
        }

        FD_ZERO(&set);
        FD_SET(param->fd, &set);
        ret = select(param->fd + 1, &set, NULL, NULL, &time);
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
			if(FD_ISSET(param->fd,&set))
			{
				/*lock*/
				ret = pthread_mutex_trylock(&(param->recvMutex));
				while (ret != 0)
				{
					ret = pthread_mutex_trylock(&(param->recvMutex));
					usleep(100);
				}
				
				param->recvLen = tc_Read(param->fd, param->recvData, RECV_DATA_LEN);
				pthread_mutex_unlock(&(param->recvMutex));
		
				tcpDbg("got %d data\n",param->recvLen);
				/*unlock*/
				if (param->recvLen < 0)
					tcpDbg("tcp read error len = %d", param->recvLen);
				else if (param->recvLen == 0)
				{
					tcpDbg("client socket disconnect %x", param);
					param->isConnect = 0;
					tc_Disconnect(param->fd);
					param->fd = 0;
				}
				continue;
			}
        }
    }
    return NULL;
}

int8_t tcpClientInit(void *tParam)
{
    int8_t ret = 0;
    struct tcpParam *param = NULL;
    if (NULL == tParam)
    {
        tcpDbg("input param error");
        return -1;
    }
    param = tParam;
    ret = tc_Init(param);
    if (param->fd < 0)
    {
        tcpDbg("tc_Init error");
        return -2;
    }
    ret = tc_Connect(param);
    if (ret < 0)
    {
        tcpDbg("connect fail");
        return -3;
    }
    /*pthread*/
    if (param->fd > 0)
    {
		param->isConnect = 1;
        pthread_create(&(param->pId), NULL, thread_start, param);
    }
    else
    {
        tcpDbg("fd error we cant read or write through this\n");
        return -4;
    }
}

int32_t tcpClientRead(struct tcpParam *param, uint8_t *buff, int32_t size)
{
    int32_t cpSize = 0;
    int32_t ret = 0;
    if (buff == NULL || param == NULL)
    {
        tcpDbg("input param error");
        return -1;
    }
    /*lock*/
    ret = pthread_mutex_trylock(&(param->recvMutex));
    while (ret != 0)
    {
        ret = pthread_mutex_trylock(&(param->recvMutex));
        usleep(100);
    }
    cpSize = param->recvLen > size ? size : param->recvLen;
    memcpy(buff, param->recvData, cpSize);
	param->recvLen = 0;
	memset(param->recvData,0,sizeof(param->recvData));
    pthread_mutex_unlock(&(param->recvMutex));

    /*unlock*/
    return cpSize;
}

int8_t tcpClientWrite(struct tcpParam *param, uint8_t *buff, int32_t size)
{
    int32_t ret = 0;
    int32_t cpSize = 0;
    if (buff == NULL || param == NULL)
    {
        tcpDbg("input param error");
        return -1;
    }
    cpSize = SEND_DATA_LEN > size ? size : SEND_DATA_LEN;
    /*lock*/
    ret = pthread_mutex_trylock(&(param->sendMutex));
    while (ret != 0)
    {
        ret = pthread_mutex_trylock(&(param->sendMutex));
        usleep(100);
    }
    memcpy(param->sendData, buff, cpSize);
    pthread_mutex_unlock(&(param->sendMutex));
    param->sendLen = cpSize;
    /*unlock*/
    return cpSize;
}

int8_t getDefaultParam(struct tcpParam **tParam)
{
    struct tcpParam *param = NULL;
    int32_t ret = 0;
    if (*tParam != NULL)
    {
        tcpDbg("param already exist");
        return -1;
    }

    *tParam = malloc(sizeof(struct tcpParam));
    param = *tParam;
    if (param == NULL)
    {
        tcpDbg("malloc error");
        return -1;
    }
    memset(param, 0, sizeof(struct tcpParam));
    param->isConnect = 0;
    param->ip = NULL;
    param->port = 0;
    param->fd = 0;
    param->pId = 0;

    ret = pthread_mutex_init(&param->recvMutex, NULL);
    if (ret < 0)
    {
        tcpDbg("mutex init fail for recv lock");
        return -2;
    }
    ret = pthread_mutex_init(&param->sendMutex, NULL);
    if (ret < 0)
    {
        tcpDbg("mutex init fail for send lock");
        return -3;
    }

    memset(param->recvData, 0, RECV_DATA_LEN);
    param->recvLen = 0;

    memset(param->sendData, 0, SEND_DATA_LEN);
    param->sendLen = 0;

    return 0;
}




