#ifndef __TCP_H__
#define __TCP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#define __DEBUG__
#ifdef __DEBUG__
#define tcpDbg printf
#else
#define tcpDbg
#endif
#define IP_MAX_LEN 16
#define RECV_DATA_LEN 256
#define SEND_DATA_LEN 256
struct tcpParam
{
    uint8_t isConnect;
    uint8_t *ip;
    uint16_t port;
    int32_t fd;
    pthread_t pId;
    uint8_t recvData[RECV_DATA_LEN];
    int32_t recvLen;
    uint8_t sendData[RECV_DATA_LEN];
    int32_t sendLen;
    pthread_mutex_t recvMutex;
    pthread_mutex_t sendMutex;
};

#endif