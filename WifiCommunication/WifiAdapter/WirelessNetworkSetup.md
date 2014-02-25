Adapter: Edimax EW-7811UN 150Mbps

Commands to run:
$ lsusb //ensures adapter plugged into Pi -- looking for something like RTL8187B
$ apt-get update //updates all directories
$ apt-cache search RTL818
$ sudo apt-get install firmware-realtek //updates firmware
$ lsmod //ID should be listed
$ sudo nano /etc/wpa.config
	network={
	ssid="YOUR_SSID"
	proto=RSN
	key
