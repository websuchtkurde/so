#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd);
void send_file_to_server(const char *filename);

int main()
{
    send_file_to_server("send.txt");
    return 0;
}

void send_file_to_server(const char *filename)
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int connection_checker;

    int sockfd;
    struct sockaddr_in server_addr;
    FILE *fp;

    // Tworzenie gniazda
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        perror(" - Error in socket. Exiting . . .");
        exit(1);
    }
    printf(" + Server socket created.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // Łączenie z serwerem
    connection_checker = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(connection_checker == -1) {
        perror(" - Error in connection. Exiting . . .");
        exit(1);
    }
    printf(" + Connected to server.\n");

    // Otwieranie pliku
    fp = fopen(filename, "r");
    if(fp == NULL) {
        perror(" - Error in reading file. Exiting . . .");
        exit(1);
    }

    // Wysyłanie pliku
    send_file(fp, sockfd);
    printf(" + File data sent successfully.\n");
}

void send_file(FILE *fp, int sockfd)
{
    //int n;
    char data[SIZE] = {0};

    while(fgets(data, SIZE, fp) != NULL) {
        if(send(sockfd, data, sizeof(data), 0) == -1) {
            perror(" - Error in sending file. Exiting . . .");
            exit(1);
        }
        bzero(data, SIZE);
    }
}
