#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#define SERVER_PORT 1096
#define SERVER_IP_ADDRESS "127.0.0.1"

void send_message(int socket_descriptor)
{
    const char *message = "Komunikat przesłany przez sieć.";
    if(send(socket_descriptor, message, strlen(message), 0) < 0)
        perror("send");
}

void receive_acknowledgement(int socket_descriptor)
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

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_descriptor < 0)
        perror("socket");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

    if(connect(socket_descriptor, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
        perror("connect");

    send_message(socket_descriptor);
    receive_acknowledgement(socket_descriptor);

    if(close(socket_descriptor) < 0)
        perror("close");
    return 0;
}
