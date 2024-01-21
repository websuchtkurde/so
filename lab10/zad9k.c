#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

void dosth(int sockfd) {
    int n;
    char buffer[256];

    printf("Podaj liczbę: ");
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Błąd zapisu do gniazda");
        exit(1);
    }
}

int create_socket() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Błąd otwarcia gniazda");
        exit(1);
    }
    return sockfd;
}

struct sockaddr_in create_serv_addr() {
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1096);
    return serv_addr;
}

void connect_to_server(int sockfd, struct sockaddr_in serv_addr) {
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Błąd połączenia");
        exit(1);
    }
}

int main() {
    int sockfd = create_socket();
    struct sockaddr_in serv_addr = create_serv_addr();

    connect_to_server(sockfd, serv_addr);

    dosth(sockfd);

    close(sockfd);
    return 0;
}
