#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int new_sock, const char *filename);
void receive_file_from_client(const char *filename);

int main()
{
    receive_file_from_client("recv.txt");
    return 0;
}

void receive_file_from_client(const char *filename)
{
    char *ip = "127.0.0.1";
    int port = 8080;
    int binder_checker;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;

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

    // Bindowanie
    binder_checker = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(binder_checker < 0) {
        perror(" - Error in binding. Exiting . . .");
        exit(1);
    }
    printf(" + Binding successul.\n");

    // Listen
    binder_checker = listen(sockfd, 10);
    if(binder_checker == 0) {
        printf(" + Listening . . .\n");
    } else {
        printf(" - Error in listening. Exiting . . .");
        exit(1);
    }
    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size); // Accept

    // Odbieranie pliku
    write_file(new_sock, filename);
    printf(" + File data received and written successfully.\n");
}

void write_file(int new_sock, const char *filename)
{
    int n;
    FILE *fp;
    char buffer[SIZE];

    fp = fopen(filename, "w");
    if(fp == NULL) {
        perror(" - Error in opening file. Exiting . . .");
        exit(1);
    }
    while(1) {
        n = recv(new_sock, buffer, SIZE, 0);
        if(n <= 0){
            break;
            return;
        }
        fprintf(fp, "%s", buffer);
        bzero(buffer, SIZE);
    }
    return;
}
