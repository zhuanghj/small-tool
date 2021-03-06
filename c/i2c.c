/*****************************************************************************
  i2c读函数，参数1：从设备地址，参数2：寄存器地址，参数3：读取数据缓冲区，参数4：读取数据大小
******************************************************************************/
unsigned char _i2c_read(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
    int fd, ret;
    unsigned char buftmp[32];
    struct i2c_rdwr_ioctl_data i2c_data;
    const char      *i2c_dev = "/dev/i2c-0";
    //----------------------------------

    device_addr >>= 1;
    //init
    fd = open(i2c_dev, O_RDWR);
    if (fd<0)
    {
        printf("not have /dev/i2c-0 t\r\n");
        return -1;
    }

    i2c_data.nmsgs = 2;
    i2c_data.msgs = (struct i2c_msg *)malloc(i2c_data.nmsgs *sizeof(struct i2c_msg));
    if (i2c_data.msgs == NULL)
    {
        printf("malloc error");
        close(fd);
        return -1;
    }

    ioctl(fd, I2C_TIMEOUT, 1);
    ioctl(fd, I2C_RETRIES, 2);

    //write reg
    memset(buftmp, 0, 32);
    buftmp[0] = sub_addr;
    i2c_data.msgs[0].len = 1;
    i2c_data.msgs[0].addr = device_addr;
    i2c_data.msgs[0].flags = 0;     // 0: write 1:read
    i2c_data.msgs[0].buf = buftmp;
    //read data
    i2c_data.msgs[1].len = ByteNo;
    i2c_data.msgs[1].addr = device_addr;
    i2c_data.msgs[1].flags = 1;     // 0: write 1:read
    i2c_data.msgs[1].buf = buff;


    ret = ioctl(fd, I2C_RDWR, (unsigned long)&i2c_data);
    if (ret < 0)
    {
        printf("read data %x %x error\r\n", device_addr, sub_addr);
        close(fd);
        free(i2c_data.msgs);
        return 1;
    }
    free(i2c_data.msgs);
    close(fd);

#if 1
    int i;
    printf("i2c__read 0x%02x:",buftmp[0]);
    for (i = 0; i < ByteNo; i++)
    {
    printf(" 0x%02x",buff[i]);
    }
    printf("\n");
#endif

    return 0;
}

/*****************************************************************************
  i2c写函数，参数1：从设备地址，参数2：寄存器地址，参数3：要写入的数据缓冲区，参数4：写入数据大小
******************************************************************************/
unsigned char _i2c_write(unsigned char device_addr, unsigned char sub_addr, unsigned char *buff, int ByteNo)
{
    int fd, ret;
    unsigned char buftmp[32];
    struct i2c_rdwr_ioctl_data i2c_data;
    const char      *i2c_dev = "/dev/i2c-0";
    //----------------------------------

    device_addr >>= 1;
    //init
    fd = open(i2c_dev, O_RDWR);
    if (fd < 0)
    {
        printf("not have /dev/i2c-0\r\n");
        return -1;
    }

    i2c_data.nmsgs = 1;
    i2c_data.msgs = (struct i2c_msg *)malloc(i2c_data.nmsgs *sizeof(struct i2c_msg));
    if (i2c_data.msgs == NULL)
    {
        printf("malloc error");
        close(fd);
        return -1;
    }

    ioctl(fd, I2C_TIMEOUT, 1);
    ioctl(fd, I2C_RETRIES, 2);

    memset(buftmp, 0, 32);
    buftmp[0] = sub_addr;
    memcpy(buftmp + 1, buff, ByteNo);
    i2c_data.msgs[0].len = ByteNo + 1;;
    i2c_data.msgs[0].addr = device_addr;
    i2c_data.msgs[0].flags = 0;     // 0: write 1:read
    i2c_data.msgs[0].buf = buftmp;
    ret = ioctl(fd, I2C_RDWR, (unsigned long)&i2c_data);
    if (ret < 0)
    {
        printf("write reg %x %x error\r\n", device_addr, sub_addr);
        close(fd);
        free(i2c_data.msgs);
        return 1;
    }
    free(i2c_data.msgs);
    close(fd);

#if 1
    int i;
    printf("i2c_write 0x%02x:",buftmp[0]);
    for(i=0; i<ByteNo; i++)
    {
    printf(" 0x%02x",buftmp[1+i]);
    }
    printf("\n");
#endif
    _alpu_delay_ms(100);
    return 0;
}


/*****************************************************************************
  延时函数（ms）
******************************************************************************/
void _alpu_delay_ms(unsigned int i)  
{
    usleep(2000 * i);
}