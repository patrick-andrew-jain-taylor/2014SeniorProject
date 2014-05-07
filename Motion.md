Motion README:
Motion is utilized in our expo presentation, demonstrating the potential functionality of our system. Using bash scripting, we accomplish the following in order:
1) Motion takes snapshot every X seconds
2) Bash script on RPi1 detects files added to snapshot folder and prints those names to stdout
3) Stdout is piped as stdin into file parser motionsend.c
4) Motionsend.c (RPI1) sends the file over a socket to Motionrecv.c (RPI2)
5) RPI2 writes file to directory
6) Bash script on RPi2 detects files added to motion folder and writes file to framebuffer