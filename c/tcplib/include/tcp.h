#ifndef __TCP_H__
#define __TCP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#define tcpDbg printf
#define IP_MAX_LEN 16
#define RECV_DATA_LEN 256
struct tcpParam
{
    uint8_t isConnect;
    uint8_t ip[IP_MAX_LEN];
    uint16_t port;
    int32_t fd;
    pthread_t pId;
    uint8_t recvData[RECV_DATA_LEN];
    int32_t recvLen;
};

#endif