// Server side C/C++ program to demonstrate Socket programming
// SOURCE: https://www.geeksforgeeks.org/socket-programming-cc/

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#define PORT 8080

int main(int argc, char const *argv[])
{
    printf("in Main\n");
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char hello[18]  = "Hello from server";

    printf("Starting server\n");
    printf("Creating Socket\n");

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    printf("setsockopt\n");
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    assert(inet_pton(AF_INET, argv[2], &address.sin_addr) > 0);
    address.sin_port = htons( 8080 );

    printf("Binding server socket:");
    printf("%s\n", argv[2]);
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        // try again after closing server
        close(server_fd);
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    }

    printf("Listening\n");
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Accepting\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("reading");
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");

    close(server_fd);
    printf("Closing socket\n");

    return 0;
}