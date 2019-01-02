ProxyDNS v1.0.2
===============
Tool written in C which bypasses DNS-based internet censorship even when port 53 is intercepted. Runs on all POSIX compliant OSes. Includes its own tiny single-purpose Linux distribution for secure and efficient use on the Raspberry Pi.

# [Download for Raspberry Pi](https://github.com/parrotgeek1/ProxyDNS/raw/master/rpi-release.zip)
* Extract the contents of this zip to a FAT32 formatted SD card and insert it into your Raspberry Pi (B/B+/2/3/3B+). 
* Only supports Ethernet, not WiFi!
# [Source Code](https://github.com/parrotgeek1/ProxyDNS)

You *must* run rpi_image.sh with sudo!

Bug
===

* It never renews the DHCP lease (on embedded version)
* It doesn't support IPv6

Servers
-------

Google
------

8.8.8.8 5353 / 8.8.4.4 5353

Unlocator
---------

https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking

185.37.37.37 54 / 185.37.39.39 54

Smart DNS Proxy
---------------

http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router

23.21.43.50 1512
54.229.171.243 1512
