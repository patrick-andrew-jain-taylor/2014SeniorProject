#Credit to https://wiki.python.org/moin/TcpCommunication
#!/usr/bin/env python

import socket

#Establish an IP, a port, and a buffer size for TCP Communication
TCP_IP = '127.0.0.1'
TCP_PORT = 5005
BUFFER_SIZE = 1024
#Message is optional, but useful for this example
MESSAGE = "Hello, World!"
#Establish a socket over the internet
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#Connect to the server
s.connect((TCP_IP, TCP_PORT))
#Send the message
s.send(MESSAGE)
#Listen for a receiving message
data = s.recv(BUFFER_SIZE)
#Close connection 
s.close()

#Print the message to the command line
print "received data:", data