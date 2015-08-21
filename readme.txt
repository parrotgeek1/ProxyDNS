jproxydns, but in c, and way better
===================================

You *must* run rpi_image.sh with sudo!
--------------------------------------

to find dns that's on other ports

https://www.google.com/search?client=safari&rls=en&ie=UTF-8&oe=UTF-8&q=%22iptables+-t+nat+-A+PREROUTING+-i+br0+-p+udp+--dport+53+-j+DNAT%22+-nvram&tbs=li 1&filter=0

go to last page click show all results

also http://wiki.opennicproject.org/Tier2 find 5353

servers in order of best to worst:

https://support.unlocator.com/customer/portal/articles/1440517-how-to-bypass-dns-hijacking

185.37.37.37 54
185.37.39.39 54

https://getflix.zendesk.com/hc/en-gb/articles/203482554-Can-I-bypass-transparent-DNS-filters-and-hijacking-with-DD-WRT-Linux-routers-

go to https://www.getflix.com.au/setup/overview
all are port 5300

http://help.unotelly.com/support/solutions/articles/199416-how-to-bypass-dns-filters-hijacking-with-dd-wrt

122.248.238.233 5353
46.165.219.110 5353


http://support.smartdnsproxy.com/customer/portal/articles/1666197-bypass-transparent-dns-proxy-with-a-dd-wrt-router

23.21.43.50 1512
54.229.171.243 1512

to do
-----

optimize/more multithread?
iptables? http://unix.stackexchange.com/questions/144482/iptables-to-redirect-dns-lookup-ip-and-port
support ipv6 server&client
add error checking for atoi etc
re-add working config file loader