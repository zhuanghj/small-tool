import socket
import time
print("tcp server start")
s = socket.socket()
host = socket.gethostname()
s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
port = 13544
s.bind((host, port))

s.listen(5)
backString = "OK"

while True:
    c, addr = s.accept()
    print("hello ", addr)
    while c:
        if c != 0:
            try:
                buff = c.recv(1024)
                if buff.__len__() > 0:
                        print(time.asctime(time.localtime(time.time())), "receive : ", end='')
                        for i in range(0, len(buff)):
                            print("%02x " % (buff[i]), end='')
                        print("")
                else:
                        c.close()
            except socket.error:
                print("socket connect error")
                c.close()
                c = 0
                continue

        if c != 0:
            try:
                c.send(backString.encode())
            except socket.error:
                print("socket connect error")
                c.close()
                c = 0
                continue


