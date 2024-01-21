#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<string.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#define SERVER_PORT 1096
#define SERVER_IP_ADDRESS "127.0.0.1"

void send_message(int socket_descriptor)
{
    struct in_addr network_address;

    if(!inet_aton(SERVER_IP_ADDRESS,&network_address))
        perror("inet_aton");

    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = SERVER_PORT,
        .sin_addr = network_address
    };

    const char *message = "Komunikat przesłany przez sieć.";
    if(sendto(socket_descriptor,message,strlen(message),0,
        (struct sockaddr *)&server_address,sizeof(server_address))<0)
        perror("sendto");
}

void receive_acknowledgement(int socket_descriptor)
{
    char buffer[512];
    struct sockaddr_in client_address;
    socklen_t address_length = sizeof(client_address);

    int received_bytes = recvfrom(socket_descriptor,(void *)buffer,sizeof(buffer),0,(struct sockaddr*)&client_address,&address_length);
    if(received_bytes<0)
        perror("recvfrom");
    else {
        buffer[received_bytes]='\0';
        puts(buffer);
    }
}

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_descriptor<0)
        perror("socket");

    send_message(socket_descriptor);
    receive_acknowledgement(socket_descriptor);

    if(close(socket_descriptor)<0)
        perror("close");
    return 0;
}
