#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define PORT 1096

void name_socket(int socket_descriptor)
{
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        perror("bind");
}

void get_and_print_message(int socket_descriptor)
{
    char buffer[512];
    int received_bytes = recv(socket_descriptor, buffer, sizeof(buffer), 0);
    if(received_bytes < 0)
        perror("recv");
    else {
        buffer[received_bytes] = '\0';
        puts(buffer);
    }
}

void send_acknowledgement(int socket_descriptor)
{
    const char *message = "Wiadomość dostarczona.";
    if(send(socket_descriptor, message, strlen(message), 0) < 0)
        perror("send");
}

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor < 0)
        perror("socket");

    name_socket(socket_descriptor);

    if(listen(socket_descriptor, 1) < 0)
        perror("listen");

    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);
    int client_socket = accept(socket_descriptor, (struct sockaddr*)&client_address, &address_length);
    if(client_socket < 0)
        perror("accept");

    get_and_print_message(client_socket);
    send_acknowledgement(client_socket);

    if(close(socket_descriptor) < 0)
        perror("close");
    return 0;
}
