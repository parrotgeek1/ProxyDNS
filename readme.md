# ProxyDNS v1.0.2 #

Tool written in C which bypasses DNS-based internet censorship even when port 53 is intercepted. Runs on all POSIX compliant OSes. 
[Source Code](https://github.com/parrotgeek1/ProxyDNS)

# Bug #

* It never renews the DHCP lease (on embedded version)
* It doesn't support IPv6

# Servers: #

## OpenDNS ##

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

HOST:185.121.177.177, 169.239.202.202

PORT: 53, 54, 443, 1053, 1194, 5353, 8080, 27015

# Usage: ##

> ProxyDNS v1.0.2, built on Jan 19 2019 at 15:40:09
> 
> Usage: proxydns host port lport [-d]

    host: as HOST, is upsteam dns server ip address.
    
    port: as PORT, is upsteam dns server port.
    
    lport: local listen port.
    
    -d: run proxydns into daemon.