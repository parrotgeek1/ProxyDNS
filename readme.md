# ProxyDNS v1.0.3 #

Tool written in C which bypasses DNS-based internet censorship even when port 53 is intercepted. Runs on all POSIX compliant OSes. Includes its own tiny single-purpose Linux distribution for secure and efficient use on the Raspberry Pi.

# [Download for Raspberry Pi](https://github.com/parrotgeek1/ProxyDNS/raw/master/rpi-release.zip)
* Extract the contents of this zip to a FAT32 formatted SD card and insert it into your Raspberry Pi (B/B+/2/3/3B+). 
* Only supports Ethernet, not WiFi!
# [Source Code](https://github.com/parrotgeek1/ProxyDNS)

You *must* run rpi_image.sh with sudo!

# Bugs #

* It never renews the DHCP lease (on the embedded version)
* It doesn't support IPv6

# Servers #

## OpenDNS (default for Raspberry Pi) ##

[OpenDNS also supports UDP and TCP over ports 443 and 5353](https://support.opendns.com/hc/en-us/community/posts/115019265903/comments/115004948686)

HOST: 208.67.222.222, 208.67.220.220, 208.67.222.123, 208.67.220.123 | Anycast

PORT: 443, 5353

## Unlocator ##

[How to bypass DNS hijacking](https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking)

HOST: 185.37.37.37, 185.37.39.39 | USA

PORT: 54

## Smart DNS Proxy ##

[Bypass transparent DNS proxy with a DD-WRT router](http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router)

HOST: 23.21.43.50, 54.229.171.243 | USA

PORT: 1512

## OpenNIC ##

[OpenNIC Public Servers](https://servers.opennicproject.org/)

1. Anycast

HOST:185.121.177.177, 169.239.202.202

PORT: 53, 54, 443, 1053, 1194, 5353, 8080, 27015

2. Australia

HOST: 103.236.162.119

PORT: 443, 5353

3. Canada

HOST: 142.4.204.111, 142.4.205.47

PORT: 443

4. Germany

HOST: 82.141.39.32, 50.3.82.162

PORT: 5353

5. France

HOST: 163.172.168.171, 91.121.84.121, 195.154.226.236

PORT: 5353

6. Russia

HOST: 5.189.170.196

PORT: 1512, 3074

7. UK

HOST: 159.89.249.249

PORT:80, 443, 5353

8. UK

HOST: 185.164.136.225

PORT: 5353

9. Romania

HOST: 188.213.49.35

PORT: 5353

## AdGuard ##
[AdGuard DNS](https://github.com/AdguardTeam/AdguardForiOS/issues/528#issuecomment-345700923)

HOST: 176.103.130.130, 176.103.130.131, 176.103.130.132, 176.103.130.134 | Russia

PORT: 5353

## 中国科学技术大学Linux 用户协会 (USTC Linux User Group) ##

HOST: 202.141.178.13, 202.141.162.123, 202.38.93.94 | China

HOST: 5353

# Usage ##
 
    Usage: proxydns host port lport [-d]

    host: upstream DNS server IP address. default on Raspberry Pi: 208.67.222.222 (OpenDNS)
    
    port: upstream DNS server port. default on Raspberry Pi: 443
    
    lport: local listen port. default on Raspberry Pi: 53
    
    -d: run ProxyDNS as a daemon.
