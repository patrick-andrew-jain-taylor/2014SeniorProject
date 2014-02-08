#Credit to https://wiki.python.org/moin/UdpCommunication
#This is a basic configuration of sending a message over UDP

import socket

#Need to define an IP as well as a port to be communicated with
UDP_IP = "127.0.0.1"
UDP_PORT = 5005
#Message is optional, but useful for this example
MESSAGE = "Hello, World!"

#This is sent out to the command line
print "UDP target IP:", UDP_IP
print "UDP target port:", UDP_PORT
print "message:", MESSAGE

#This establishes a socket
sock = socket.socket(socket.AF_INET, # Internet
                     socket.SOCK_DGRAM) # UDP
#Once socket is established, message is sent over UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))