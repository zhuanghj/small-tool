import socket
print("tcp server start")
s = socket.socket()
host = socket.gethostname()
port = 13544
s.bind((host, port))

s.listen(5)
backString = "OK"

while True:
    c, addr = s.accept()
    print("hello ", addr)
    while c:
        try:
            buff = c.recv(1024)

        except socket.error:
            print("socket connect error")
            c.close()
            c = 0
            continue
        if c != 0:
            if buff.__len__() > 0:
                print("receive : ", buff)
            try:
                c.send(backString.encode())
            except socket.error:
                print("socket connect error")
                c.close()
                c = 0
                continue



