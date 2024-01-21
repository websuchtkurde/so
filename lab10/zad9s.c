#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

void dosth(char* buffer) {
    printf("Oto wiadomość: %s\n", buffer);
}

void start_server(const char *ip, int port) {
    int sockfd, newsockfd, max_sd, sd, activity, i, valread;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    fd_set readfds;
    int client_socket[30];
    char buffer[1024];

    for (i = 0; i < 30; i++) {
        client_socket[i] = 0;
    }

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
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        max_sd = sockfd;

        for (i = 0; i < 30; i++) {
            sd = client_socket[i];
            if (sd > 0) {
                FD_SET(sd, &readfds);
            }
            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        if ((activity < 0) && (errno != EINTR)) {
            printf("Błąd select\n");
        }

        if (FD_ISSET(sockfd, &readfds)) {
            if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) < 0) {
                perror("Błąd akceptacji");
                exit(EXIT_FAILURE);
            }

            for (i = 0; i < 30; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = newsockfd;
                    break;
                }
            }
        }

        for (i = 0; i < 30; i++) {
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1023)) == 0) {
                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    dosth(buffer);
                }
            }
        }
    }

    close(sockfd);
}

int main() {
    start_server("127.0.0.1", 1096);
    return 0;
}
