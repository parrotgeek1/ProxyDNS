ProxyDNS v1.0.1
===============
A tool to bypass DNS-based internet censorship when port 53, or even DNS packets themselves, are blocked by a firewall.

You *must* run rpi_image.sh with sudo!
--------------------------------------

Bugs
====

* no udp connection tracking (for > 1 packet queries), tcp is ok

* no multithreading (slower)

* no error checking for atoi etc

* It never renews the DHCP lease (on embedded version)

To find DNS servers not on port 53
==================================

* https://www.google.com/search?client=safari&rls=en&ie=UTF-8&oe=UTF-8&q=%22iptables+-t+nat+-A+PREROUTING+-i+br0+-p+udp+--dport+53+-j+DNAT%22+-nvram&tbs=li&filter=0

* go to last page click show all results

* also http://wiki.opennicproject.org/Tier2 find 5353

Servers in order of best to worst:
----------------------------------

Unlocator
---------

https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking

185.37.37.37 54 / 185.37.39.39 54

Getflix
-------

https://getflix.zendesk.com/hc/en-gb/articles/203482554-Can-I-bypass-transparent-DNS-filters-and-hijacking-with-DD-WRT-Linux-routers-

go to https://www.getflix.com.au/setup/overview - all are port 5300

Unotelly
--------

http://help.unotelly.com/support/solutions/articles/199416-how-to-bypass-dns-filters-hijacking-with-dd-wrt

122.248.238.233 5353 / 46.165.219.110 5353

Smart DNS Proxy
---------------

http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router

23.21.43.50 1512
54.229.171.243 1512

To Do
=====

iptables http://unix.stackexchange.com/questions/144482/iptables-to-redirect-dns-lookup-ip-and-port

support ipv6 server&client

re-add working config file loader

klibc static

web interface for config dhcp or not, etc

use a random server from a list

support a5-v11, wt1520/3020? etc

use smaller kernel