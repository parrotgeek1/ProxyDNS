#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

#define PORT     "53" /* Port to listen on */
#define BACKLOG  10      /* Passed to listen() */
#define BUF_SIZE 4096    /* Buffer for  transfers */

unsigned int transfer(int from, int to)
{
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

void handle(int client, char *host, char *port)
{
    struct addrinfo hints, *res;
    int server = -1;
    unsigned int disconnected = 0;
    fd_set set;
    unsigned int max_sock;
    
    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, port, &hints, &res) != 0) {
        perror("getaddrinfo");
        close(client);
        return;
    }
    
    /* Create the socket */
    server = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server == -1) {
        perror("socket");
        close(client);
        return;
    }
    
    /* Connect to the host */
    if (connect(server, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(client);
        return;
    }
    
    if (client > server) {
        max_sock = client;
    }
    else {
        max_sock = server;
    }
    
    /* Main transfer loop */
    while (!disconnected) {
        FD_ZERO(&set);
        FD_SET(client, &set);
        FD_SET(server, &set);
        if (select(max_sock + 1, &set, NULL, NULL, NULL) == -1) {
            perror("select");
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

void udpthread(char *ip, char* port) {
    int os=socket(PF_INET,SOCK_DGRAM,IPPROTO_IP);
    
    struct sockaddr_in a;
    a.sin_family=AF_INET;
    a.sin_addr.s_addr=inet_addr("0.0.0.0"); a.sin_port=htons(53);
    if(bind(os,(struct sockaddr *)&a,sizeof(a)) == -1) {
        printf("udp Can't bind our address\n");
        exit(1); }
    
    a.sin_addr.s_addr=inet_addr(ip); a.sin_port=htons(atoi(port));
    
    struct sockaddr_in sa;
    struct sockaddr_in da; da.sin_addr.s_addr=0;
    while(1) {
        char buf[256];
        socklen_t sn=sizeof(sa);
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


int main(int argc, char **argv)
{
    int sock;
    struct addrinfo hints, *res;
    int reuseaddr = 1; /* True */
    char * host, * port;
    
    /* Get the server host and port from the command line */
    if (argc < 3) {
        fprintf(stderr, "Usage: proxydns2 host port\n");
        return 1;
    }
    host = argv[1];
    port = argv[2];
    
    /* Get the address info */
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        return 1;
    }
    
    /* Create the socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) {
        perror("socket");
        freeaddrinfo(res);
        return 1;
    }
    
    /* Enable the socket to reuse the address */
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("setsockopt");
        freeaddrinfo(res);
        return 1;
    }
    
    /* Bind to the address */
    if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        freeaddrinfo(res);
        return 1;
    }
    
    /* Listen */
    if (listen(sock, BACKLOG) == -1) {
        perror("listen");
        freeaddrinfo(res);
        return 1;
    }
    
    freeaddrinfo(res);
    
    /* Ignore broken pipe signal */
    signal(SIGPIPE, SIG_IGN);
    pid_t pid = fork();
    
    if (pid == 0)
    {
        // child process
        udpthread(host,port);
    }
    else if (pid > 0)
    {
        // parent process
        /* Main loop */
        while (1) {
            socklen_t size = sizeof(struct sockaddr_in);
            struct sockaddr_in their_addr;
            int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);
            
            if (newsock == -1) {
                perror("accept");
            }
            else {
                handle(newsock, host, port);
            }
        }
        
        close(sock);
    }
    else
    {
        // fork failed
        printf("fork() failed!\n");
        close(sock);
        return 1;
    }
    
    return 0;
}
