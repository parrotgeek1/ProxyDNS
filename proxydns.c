#ifdef EMBEDDED
#define CFG_HOST "208.67.222.222"
#define CFG_PORT 443
#define CFG_IP "ip=dhcp"
#define CFG_LPORT 53
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>

#define BACKLOG  10      /* Passed to listen() */
#define BUF_SIZE 65535    /* Maximum size of DNS packet */

#ifdef EMBEDDED
#include <sys/utsname.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>

extern int nice(int incr);

static void unameinfo(void) {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        perror("uname");
        return;
    }
    printf("Running on %s %s %s %s\n", buffer.sysname,buffer.release,buffer.version,buffer.machine);
}
#endif

static unsigned int transfer(int from, int to) {
    char buf[BUF_SIZE];
    unsigned int disconnected = 0;
    ssize_t bytes_read, bytes_written;
    bytes_read = read(from, buf, BUF_SIZE);
    if (bytes_read <= 0) {
        disconnected = 1;
    }
    else {
        bytes_written = write(to, buf, (size_t)bytes_read);
        if (bytes_written != bytes_read) {
            disconnected = 1;
        }
    }
    return disconnected;
}

static void handle(int client, in_addr_t host, uint16_t htonsport) {
    int server = -1;
    unsigned int disconnected = 0;
    fd_set set;
    int max_sock;
    struct sockaddr_in serv_addr;

    server = socket(PF_INET,SOCK_STREAM,IPPROTO_IP);
    if (server == -1) {
        perror("TCP error: socket");
        close(client);
        return;
    }
    serv_addr.sin_addr.s_addr = host;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htonsport;

    if (connect(server, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("TCP error: connect");
        close(client);
        return;
    }

    if (client > server) {
        max_sock = client;
    } else {
        max_sock = server;
    }

    while (!disconnected) {
        FD_ZERO(&set);
        FD_SET(client, &set);
        FD_SET(server, &set);
        if (select(max_sock + 1, &set, NULL, NULL, NULL) == -1) {
            perror("TCP error: select");
            break;
        }
        if (FD_ISSET(client, &set)) {
            disconnected = transfer(client, server);
        } else if (FD_ISSET(server, &set)) {
            disconnected = transfer(server, client);
        }
    }
    close(server);
    close(client);
}

static void udpserver(char *ip, int port, int lport) {
    int os=socket(PF_INET,SOCK_DGRAM,IPPROTO_IP);
    struct sockaddr_in a, sa, da;
    socklen_t sn=sizeof(sa);
    ssize_t n;
    char buf[65507];
    a.sin_family=AF_INET;
    a.sin_addr.s_addr=0;
    a.sin_port=htons(lport);
    if(bind(os,(struct sockaddr *)&a,sizeof(a)) == -1) {
        perror("UDP error: bind");
        exit(1);
    }
    a.sin_addr.s_addr=inet_addr(ip);
    a.sin_port=htons(port);
    da.sin_addr.s_addr=0;
    /* the actual limit for the data length, which is imposed by the underlying IPv4 protocol, is 65,507 bytes (65,535 − 8 byte UDP header − 20 byte IP header) */
    puts("Starting UDP server");
    while(1) {
        n=recvfrom(os,buf,sizeof(buf),0,(struct sockaddr *)&sa,&sn);
        if(n<=0) continue;

        if(sa.sin_addr.s_addr==a.sin_addr.s_addr && sa.sin_port==a.sin_port) {
            if(da.sin_addr.s_addr) sendto(os,buf,(size_t)n,0,(struct sockaddr *)&da,sizeof(da));
        } else {
            sendto(os,buf,(size_t)n,0,(struct sockaddr *)&a,sizeof(a));
            da=sa;
        }
    }
}

int main(int argc, char **argv) {
    int sock;
    int reuseaddr = 1;
    char *host;
    in_addr_t hostinaddr;
    int port;
    int lport;
    uint16_t htonsport;
    struct sockaddr_in atcp;
    pid_t pid;
#ifdef EMBEDDED
    pid_t pidip;
    int status;
    host = CFG_HOST;
    port = CFG_PORT;
    lport = CFG_LPORT;
#endif
    puts("ProxyDNS "
#ifdef EMBEDDED
    "OS "
#endif
    "v1.0.6");
#ifdef EMBEDDED
    printf("\x1B[1;1H\x1B[2J"); /* clear spurious vchiq errors */
    nice(-20);
    unameinfo();
    mount("none","/proc","proc", 0,NULL);
    mount("none","/sys","sysfs", 0,NULL);
    puts("Waiting 2 seconds for network device");
    sleep(2);
    pidip = fork();
    if (pidip == -1) {
        /* fork failed */
        perror("fork");
        return 1;
    } else if (pidip > 0) {
        waitpid(pidip, &status, 0);
    } else {
        /* we are the child */
        execl("/ipconfig","ipconfig",CFG_IP,NULL);
        _exit(EXIT_FAILURE);   /* exec never returns */
    }
#else
    /* Get the server host and port from the command line */
    if (argc < 4 || argc > 5 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0) {
        fprintf(stderr, "Usage: %s host port lport [-d]\n",argv[0]);
        return 1;
    }
    host = argv[1];
    port = atoi(argv[2]);
    lport = atoi(argv[3]);
#endif
    hostinaddr = inet_addr(host);
    htonsport = htons(port);
    printf("Proxying DNS server at %s port %d to local port %d\n",host,port,lport);
#ifndef EMBEDDED
    if(argc == 5 && strcmp(argv[4],"-d") == 0) {
        puts("Becoming a daemon");
        daemon(0,0);
    }
#endif
    sock = socket(PF_INET,SOCK_STREAM,IPPROTO_IP);
    if (sock == -1) {
        perror("TCP error: socket");
        return 1;
    }
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("TCP error: setsockopt");
        return 1;
    }
    atcp.sin_family=AF_INET;
    atcp.sin_addr.s_addr=0;
    atcp.sin_port=htons(lport);
    if (bind(sock, (struct sockaddr *)&atcp,sizeof(atcp)) == -1) {
        perror("TCP error: bind");
        return 1;
    }
    if (listen(sock, BACKLOG) == -1) {
        perror("TCP error: listen");
        return 1;
    }
    signal(SIGPIPE, SIG_IGN);
    pid = fork();
    if (pid == 0) {
        /* we are the child */
        udpserver(host,port,lport);
    } else if (pid > 0) {
        puts("Starting TCP server");
        while (1) {
            socklen_t size = sizeof(struct sockaddr_in);
            struct sockaddr_in their_addr;
            int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
            if (newsock == -1) {
                perror("TCP error: accept");
            } else {
                handle(newsock, hostinaddr, htonsport);
            }
        }
    } else {
        /* fork failed */
        perror("fork");
        close(sock);
        return 1;
    }
    return 0;
}
