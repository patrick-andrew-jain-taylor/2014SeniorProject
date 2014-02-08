#Credit to https://wiki.python.org/moin/UdpCommunication
#This is a basic configuration of receiving a message over UDP

import socket

#Need to define an IP as well as a port to be communicated with
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

#This establishes a socket
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
#Once established, this socket will be bound to the following IP and port
sock.bind((UDP_IP, UDP_PORT))

#Will continue to run until the socket is closed
while True:
	#Need to establish buffer size
    data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
    #Message is printed out to command line
	print "received message:", data