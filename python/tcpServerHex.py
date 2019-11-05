import socket
import time
import struct
import sys
print("tcp server start")
s = socket.socket()
host = socket.gethostname()
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
port = 13544
s.bind((host, port))

s.listen(5)
backString = "OK"
a = [0x23, 0x23, 0x01, 0x01,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x01,0x00,0x05]
while True:
    c, addr = s.accept()
    print("hello ", addr)
    while c:
        if c != 0:
            try:
                buff = c.recv(1024)
                if buff.__len__() > 0:
                    #   for python2
                        print(time.asctime(time.localtime(time.time())), "receive : "),
                        for i in range(0, len(buff)):
                            print("%02x " % (ord(buff[i]))),

                        #  for python3
                        # print(time.asctime(time.localtime(time.time())), "receive : ", end='')
                        # for i in range(0, len(buff)):
                        #     print("%02x " % (buff[i]), end='')
                        print("\n")
                else:
                    c.close()
            except socket.error:
                print("socket connect error")
                c.close()
                c = 0
                continue

        if c != 0:
            try:
                # c.send(backString.encode())
                # date = struct.pack("%dB" % (len(a)), a)
                # c.send(date)
                for i in range(a.__len__()):
                    c.send(chr(a[i]))
            except socket.error:
                print("socket connect error")
                c.close()
                c = 0
            continue


