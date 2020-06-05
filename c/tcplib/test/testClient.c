#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "unistd.h"
#include "string.h"
#include "../include/tcp.h"

int main()
{
    int32_t ret = 0;
    uint8_t buff[16] = "";
    uint8_t size = 16;
    uint8_t ip[IP_MAX_LEN] = "192.168.56.1";
    uint16_t port = 8989;
    struct tcpParam *param = NULL;
    memset(buff,0x31,sizeof(buff));
    ret = getDefaultParam(&param);
    if (ret < 0)
    {
        printf("getDefaultParam error");
        return -1;
    }
    param->ip = ip;
    param->port = port;
    ret = tcpClientInit(param);
    if (ret < 0)
    {
        printf("tcp init fail");
        return -1;
    }
    printf("start main loop\n");
    while (1)
    {
		ret = tcpClientRead(param, buff, size);
		if(ret > 0)
		{
			printf("got msg %s\n",buff);
			continue;
		}
        ret = tcpClientWrite(param, buff, size);
        sleep(1);
    }

    return 0;
}