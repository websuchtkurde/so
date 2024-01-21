#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 12345
#define MAX_CLIENTS 5
#define MAX_BUFFER_SIZE 1024

void handle_client(int client_socket);
void start_server();

int main() {
    start_server();
    return 0;
}

void handle_client(int client_socket) {
    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received message from client: %s", buffer);
    }

    close(client_socket);
}

void start_server() {
    int server_socket, client_sockets[MAX_CLIENTS], max_clients, activity;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    fd_set readfds;

    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    max_clients = MAX_CLIENTS;
    memset(client_sockets, 0, sizeof(client_sockets));

    while (1) {
        FD_ZERO(&readfds);
        FD_SET(server_socket, &readfds);

        int max_sd = server_socket;

        for (int i = 0; i < max_clients; i++) {
            int sd = client_sockets[i];

            if (sd > 0) {
                FD_SET(sd, &readfds);
            }

            if (sd > max_sd) {
                max_sd = sd;
            }
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR)) {
            perror("Select error");
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(server_socket, &readfds)) {
            int new_socket;

            if ((new_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
                perror("Accept error");
                exit(EXIT_FAILURE);
            }

            printf("New connection, socket fd is %d, IP is: %s, port is: %d\n",
                   new_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            for (int i = 0; i < max_clients; i++) {
                if (client_sockets[i] == 0) {
                    client_sockets[i] = new_socket;
                    break;
                }
            }
        }

        for (int i = 0; i < max_clients; i++) {
            int sd = client_sockets[i];

            if (sd > 0 && FD_ISSET(sd, &readfds)) {
                handle_client(sd);
                client_sockets[i] = 0;  // Reset the socket descriptor after handling the client
            }
        }
    }

    // Close all client sockets before exiting
    for (int i = 0; i < max_clients; i++) {
        int sd = client_sockets[i];
        if (sd > 0) {
            close(sd);
        }
    }

    close(server_socket);
}

