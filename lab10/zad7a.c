#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

void dosth(int newsockfd) {
    int n;
    char buffer[256];

    bzero(buffer, 256);
    n = read(newsockfd, buffer, 255);
    if (n < 0) {
        perror("Błąd odczytu z gniazda");
        exit(1);
    }
    printf("Oto wiadomość: %s\n", buffer);
}

void start_server(const char *ip, int port) {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Błąd otwarcia gniazda");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Błąd przywiązania");
        exit(1);
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) {
            perror("Błąd akceptacji");
            exit(1);
        }

        int pid = fork();
        if (pid < 0) {
            perror("Błąd przy tworzeniu procesu potomnego");
            exit(1);
        }

        if (pid == 0) {
            close(sockfd);
            dosth(newsockfd);
            exit(0);
        } else {
            signal(SIGCHLD, SIG_IGN);
            close(newsockfd);
        }
    }

    close(sockfd);
}

int main() {
    start_server("127.0.0.1", 1096);
    return 0;
}
