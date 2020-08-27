#define CFG_HOST "208.67.222.222"
#define CFG_PORT 443
#define CFG_IP "ip=dhcp"
#define CFG_LPORT 53

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BACKLOG  10      // Passed to listen()
#define BUF_SIZE 65535    // Maximum size of DNS packet

#ifdef EMBEDDED
#include <sys/utsname.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    size_t bytes_read, bytes_written;
    bytes_read = read(from, buf, BUF_SIZE);
    if (bytes_read == 0) {
        disconnected = 1;
    }
    else {
        bytes_written = write(to, buf, bytes_read);
        if (bytes_written == (size_t)-1) {
            disconnected = 1;
        }
    }
    return disconnected;
}

static void handle(int client, char *host, int port) {
    int server = -1;
    unsigned int disconnected = 0;
    fd_set set;
    unsigned int max_sock;

    server = socket(PF_INET,SOCK_STREAM,IPPROTO_IP);
    if (server == -1) {
        perror("TCP error: socket");
        close(client);
        return;
    }
    struct sockaddr_in serv_addr;
    serv_addr.sin_addr.s_addr = inet_addr(host);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (connect(server, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        perror("TCP error: connect");
        close(client);
        return;
    }

    if (client > server) {
        max_sock = client;
    }
    else {
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
        }
        if (FD_ISSET(server, &set)) {
            disconnected = transfer(server, client);
        }
    }
    close(server);
    close(client);
}

static void udpthread(char *ip, int port, int lport) {
    int os=socket(PF_INET,SOCK_DGRAM,IPPROTO_IP);

    struct sockaddr_in a;
    a.sin_family=AF_INET;
    a.sin_addr.s_addr=inet_addr("0.0.0.0");
    a.sin_port=htons(lport);
    if(bind(os,(struct sockaddr *)&a,sizeof(a)) == -1) {
        perror("UDP error: bind");
        exit(1);
    }

    a.sin_addr.s_addr=inet_addr(ip);
    a.sin_port=htons(port);
    struct sockaddr_in sa;
    socklen_t sn=sizeof(sa);
    struct sockaddr_in da;
    da.sin_addr.s_addr=0;
    puts("Started UDP thread");
    while(1) {
        char buf[256];
        int n=recvfrom(os,buf,sizeof(buf),0,(struct sockaddr *)&sa,&sn);
        if(n<=0) continue;

        if(sa.sin_addr.s_addr==a.sin_addr.s_addr && sa.sin_port==a.sin_port) {
            if(da.sin_addr.s_addr) sendto(os,buf,n,0,(struct sockaddr *)&da,sizeof(da));
        } else {
            sendto(os,buf,n,0,(struct sockaddr *)&a,sizeof(a));
            da=sa;
        }
    }
}

int main(int argc, char **argv) {
    int sock;
    int reuseaddr = 1;
    char *host;
    int port;
    int lport;
#ifdef EMBEDDED
    host = CFG_HOST;
    port = CFG_PORT;
    lport = CFG_LPORT;
#endif
    puts("ProxyDNS "
#ifdef EMBEDDED
    "OS "
#endif
    "v1.0.5");
#ifdef EMBEDDED
    printf("\e[1;1H\e[2J"); // clear spurious vchiq errors
    nice(-20);
    unameinfo();
    mount("none","/proc","proc", 0,NULL);
    mount("none","/sys","sysfs", 0,NULL);
    puts("Waiting 2 seconds for network device");
    sleep(2);
    pid_t pidip = fork();

    if (pidip == -1) {
        // fork failed
        perror("fork");
        return 1;
    } else if (pidip > 0) {
        int status;
        waitpid(pidip, &status, 0);
    } else {
        // we are the child
        execl("/ipconfig","ipconfig",CFG_IP,NULL);
        _exit(EXIT_FAILURE);   // exec never returns
    }
#else
    // Get the server host and port from the command line
    if (argc < 4 || argc > 5 || atoi(argv[2]) <= 0) {
        fprintf(stderr, "Usage: %s host port lport [-d]\n",argv[0]);
        return 1;
    }
    host = argv[1];
    port = atoi(argv[2]);
    lport = atoi(argv[3]);
#endif
    printf("Proxying DNS server from %s port %d to local port %d\n",host,port,lport);
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
    struct sockaddr_in atcp;
    atcp.sin_family=AF_INET;
    atcp.sin_addr.s_addr=inet_addr("0.0.0.0");
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
    pid_t pid = fork();
    if (pid == 0) {
        // child process
        udpthread(host,port,lport);
    } else if (pid > 0) {
        puts("Started TCP thread");
        while (1) {
            socklen_t size = sizeof(struct sockaddr_in);
            struct sockaddr_in their_addr;
            int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
            if (newsock == -1) {
                perror("TCP error: accept");
            } else {
                handle(newsock, host, port);
            }
        }
        close(sock);
    } else {
        // fork failed
        perror("fork");
        close(sock);
        return 1;
    }
    return 0;
}
