#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

void *dosth(void *newsockfd) {
    int n;
    char buffer[256] = {0};
    int new_sock = *(int*)newsockfd;

    n = read(new_sock, buffer, 255);
    if (n < 0) {
        perror("read");
    }
    printf("wiadomosc przychodzaca: %s\n", buffer);

    close(new_sock); // zamkniecie newsock

    pthread_detach(pthread_self()); // watek rozłaczny

    return 0;
}

void start_server(const char *ip, int port) {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread_id;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind");
    }

    if(listen(sockfd, 5)<0)
        perror("listen");
    clilen = sizeof(cli_addr);

    // zamiana while(1) na for
    for(int i = 0; i < 10; i++) { // przykładowa liczba 10, zmień na potrzebną ilość klientów
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("accept");
        }

        if (pthread_create(&thread_id, NULL, dosth, &newsockfd) < 0) {
            fprintf(stderr,"blad w tworzeniu watku");
        }
    }

    if(close(sockfd)<0)
    perror("close");
}

int main(void) {
    start_server("127.0.0.1", 1096);
    return 0;
}

