# ProxyDNS v1.0.3 #

Tool written in C which bypasses DNS-based internet censorship even when port 53 is intercepted. Runs on all POSIX compliant OSes.Includes its own tiny single-purpose Linux distribution for secure and efficient use on the Raspberry Pi.

# [Download for Raspberry Pi](https://github.com/parrotgeek1/ProxyDNS/raw/master/rpi-release.zip)
* Extract the contents of this zip to a FAT32 formatted SD card and insert it into your Raspberry Pi (B/B+/2/3/3B+). 
* Only supports Ethernet, not WiFi!
# [Source Code](https://github.com/parrotgeek1/ProxyDNS)

You *must* run rpi_image.sh with sudo!

# Bug #

* It never renews the DHCP lease (on embedded version)
* It doesn't support IPv6

# Servers: #

## OpenDNS ##

[ OpenDNS supports also UDP and TCP over ports 443 and 5353](https://support.opendns.com/hc/en-us/community/posts/115019265903/comments/115004948686)

HOST: 208.67.222.222, 208.67.220.220

PORT: 443, 5353

## Unlocator ##

[how-to-bypass-dns-hijacking](https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking)

HOST: 185.37.37.37, 185.37.39.39

PORT: 54

## Smart DNS Proxy ##

[bypass-transparent-dns-proxy-with-a-dd-wrt-router](http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router)

HOST: 23.21.43.50, 54.229.171.243

PORT: 1512

## OpenNIC(anycast) ##

[OpenNIC Public Servers](https://servers.opennicproject.org/)

HOST:185.121.177.177, 169.239.202.202

PORT: 53, 54, 443, 1053, 1194, 5353, 8080, 27015

## AdGuard ##
[AdGuard DNS](https://github.com/AdguardTeam/AdguardForiOS/issues/528#issuecomment-345700923)

HOST: 176.103.130.130, 176.103.130.131, 176.103.130.132, 176.103.130.134

PORT: 5353

## Google (blocked by IP in China) ##

HOST: 8.8.8.8, 8.8.4.4

PORT: 5353

# Usage: ##

> ProxyDNS v1.0.3, built on Jan 23 2019 at 23:05:11
> 
> Usage: proxydns host port lport [-d]

    host: as HOST, is upstream DNS server ip address. default:208.67.222.222 (OpenDNS)
    
    port: as PORT, is upstream DNS server port. default: 443
    
    lport: local listen port. default: 53
    
    -d: run proxydns as a daemon.
