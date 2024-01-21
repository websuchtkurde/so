#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/ip.h>
#include<string.h> // Dodajemy bibliotekę string.h
#define PORT 1096

void name_socket(int socket_descriptor)
{
    struct sockaddr_in server_address = {
        .sin_family = AF_INET,
        .sin_port = PORT,
        .sin_addr = {INADDR_ANY}
    };

    int yes = 1;
    if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        // obsługa błędu
    }

    if(bind(socket_descriptor,(struct sockaddr*)&server_address,sizeof(server_address))<0)
        perror("bind");
}

void get_and_print_message(int socket_descriptor, struct sockaddr_in *client_address, socklen_t *address_length) // Dodajemy argumenty
{
    char buffer[512];

    int received_bytes = recvfrom(socket_descriptor,(void *)buffer,sizeof(buffer),0,(struct sockaddr*)client_address,address_length);
    if(received_bytes<0)
        perror("recvfrom");
    else {
        buffer[received_bytes]='\0';
        puts(buffer);
    }
}

void send_acknowledgement(int socket_descriptor, struct sockaddr_in client_address, socklen_t address_length)
{
    const char *message = "Wiadomość dostarczona.";
    if(sendto(socket_descriptor,message,strlen(message),0,
        (struct sockaddr *)&client_address,address_length)<0)
        perror("sendto");
}

int main(void)
{
    int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_descriptor<0)
        perror("socket");

    name_socket(socket_descriptor);
    struct sockaddr_in client_address; // Przenosimy te zmienne do funkcji main
    socklen_t address_length = sizeof(client_address);
    get_and_print_message(socket_descriptor, &client_address, &address_length); // Przekazujemy zmienne jako argumenty
    send_acknowledgement(socket_descriptor, client_address, address_length);

    if(close(socket_descriptor)<0)
        perror("close");
    return 0;
}
