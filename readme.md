# ProxyDNS v1.0.6

Tool written in C which bypasses DNS-based internet censorship even when port 53 is intercepted. Runs on all POSIX compliant OSes, and is compliant with C89 for maximum portability. Includes its own tiny single-purpose Linux distribution for secure and efficient use on the Raspberry Pi.

# [Download for Raspberry Pi](https://github.com/parrotgeek1/ProxyDNS/raw/main/rpi-release.zip)
* Extract the contents of this zip to a FAT32 formatted SD card and insert it into your Raspberry Pi (B/B+/2/3/3B+/4). 
* Only supports Ethernet, not WiFi!
# [Source Code](https://github.com/parrotgeek1/ProxyDNS)

You *must* run rpi_image.sh with sudo!

# Bugs

* DHCP lease is never renewed on the Raspberry Pi version
* No support for IPv6

# Servers

## OpenDNS (default for Raspberry Pi)

Source: [OpenDNS also supports UDP and TCP over ports 443 and 5353](https://support.opendns.com/hc/en-us/community/posts/115019265903/comments/115004948686)

Host: 208.67.222.222, 208.67.220.220, 208.67.222.123, 208.67.220.123 | Anycast

Port: 443, 5353

## Quad9

Source: [My ISP captures port 53, is there another port I can use for Quad9?](https://www.quad9.net/faq/#My_ISP_captures_port_53_is_there_another_port_I_can_use_for_Quad9)

Host: 9.9.9.9

Port: 9953

## Unlocator

Source: [How to bypass DNS hijacking](https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking)

Host: 185.37.37.37, 185.37.39.39 | USA

Port: 54

## Smart DNS Proxy

Source: [Bypass transparent DNS proxy with a DD-WRT router](http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router)

Host: 23.21.43.50, 54.229.171.243 | USA

Port: 1512

## OpenNIC

Source: [OpenNIC Public Servers](https://servers.opennicproject.org/)

1. Anycast

Host: 185.121.177.177, 169.239.202.202

PORT: 53, 54, 443, 1053, 1194, 5353, 8080, 27015

2. Australia

Host: 103.236.162.119

Port: 443, 5353

3. Canada

Host: 142.4.204.111, 142.4.205.47

PORT: 443

4. Germany

Host: 82.141.39.32, 50.3.82.162

Port: 5353

5. France

Host: 163.172.168.171, 91.121.84.121, 195.154.226.236

Port: 5353

6. Russia

Host: 5.189.170.196

Port: 1512, 3074

7. UK

Host: 159.89.249.249

Port: 80, 443, 5353

8. UK

Host: 185.164.136.225

Port: 5353

9. Romania

Host: 188.213.49.35

Port: 5353

## AdGuard
Source: [AdGuard DNS GitHub issue](https://github.com/AdguardTeam/AdguardForiOS/issues/528#issuecomment-345700923)

Host: 176.103.130.130, 176.103.130.131, 176.103.130.132, 176.103.130.134 | Russia

Port: 5353

## USTC Linux User Group

Host: 202.141.178.13, 202.141.162.123, 202.38.93.94 | China

Port: 5353

# Usage
 
    Usage: proxydns host port lport [-d]

    host: upstream DNS server IP address. default on Raspberry Pi: 208.67.222.222 (OpenDNS)
    
    port: upstream DNS server port. default on Raspberry Pi: 443
    
    lport: local listening port. default on Raspberry Pi: 53
    
    -d: run ProxyDNS as a daemon.
