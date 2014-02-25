#Credit to https://wiki.python.org/moin/TcpCommunication
#!/usr/bin/env python

import socket

#Establish an IP, a port, and a buffer size for TCP Communication
TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

#Establish a socket over the internet
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Bind the socket to a TCP IP and port
s.bind((TCP_IP, TCP_PORT))
#Listen until a connection is made
s.listen(1)

#Accept the connection
conn, addr = s.accept()
#Print the accepted address
print 'Connection address:', addr
#Listen until no data is received
while 1:
	#Receive a packet
    data = conn.recv(BUFFER_SIZE)
    if not data: break
	#Print the packet
    print "received data:", data
	#Echo that message back over the socket
    conn.send(data)  # echo
#Close the connection
conn.close()