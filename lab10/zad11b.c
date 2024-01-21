#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define MAXLINE 1024

void start_client(const char *ip, int port) {
    int sockfd, n;
    struct sockaddr_in servaddr;

    char buffer[MAXLINE];
    char *hello = "Hello from client";
    
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(ip);
    
    socklen_t len; // Zmieniono 'int' na 'socklen_t'
    len = sizeof(servaddr);
    
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    
    sendto(sockfd, (const char *)hello, strlen(hello), MSG_CONFIRM, (const struct sockaddr *)&servaddr, len);
    printf("Hello message sent.\n");
    
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Server : %s\n", buffer);
    
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    
    printf("Time taken to send and receive packet: %f\n", cpu_time_used);
    
    close(sockfd);
}

int main() {
    start_client("127.0.0.1", 8080);
    return 0;
}
