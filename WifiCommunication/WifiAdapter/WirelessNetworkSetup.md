Adapter: Edimax EW-7811UN 150Mbps

Commands to run:
$ sudo /etc/network/interfaces

auto lo
 
iface lo inet loopback
iface eth0 inet dhcp
 
allow-hotplug wlan0
auto wlan0
 
 
iface wlan0 inet dhcp
        wpa-ssid "ssid" //"Test"
        wpa-psk "password" //"Glasses1"
