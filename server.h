//
// Created by kramt on 2/21/2020.
//
#include "node.h"
#include "serialize.h"
#include "string.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>

#pragma once

class Server {
public:
    size_t nodes;
    size_t * ports;  // owned
    String ** addresses;  // owned; strings owned

    //Personal Stuff
    String* ip_addr;
    int port;

    //socket stuff
    int sock_listen;
    int sock_send;

    Server(String* ip_addr, size_t port) {
        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String*[1000];
        addresses[0] = this->ip_addr;
        this->ports = new size_t[1000];
        ports[0] = this->port;
        this->nodes = 1;

        int valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};


        printf("Starting server\n");
        printf("Creating Socket\n");

        // Creating socket file descriptor
        if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        printf("setsockopt\n");
        // Forcefully attaching socket to the port 8080
        if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(this->port);

        if(inet_pton(AF_INET,"127.0.0.2", &address.sin_addr) <= 0) {
            printf("SERVER: ERROR INET");
            exit(EXIT_FAILURE);
        }

        printf("Binding server socket:");
        printf("%s\n", this->ip_addr->cstr_);
        // Forcefully attaching socket to the port 8080
        if (bind(sock_listen, (struct sockaddr *)&address, sizeof(address))<0)
        {
            int e = errno;
            printf("EXITING %d", e);
            exit(EXIT_FAILURE);
            perror("binding");
        }

        printf("Listening\n");
        if (listen(sock_listen, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        printf("Accepting\n");
        if ((sock_send = accept(sock_listen, (struct sockaddr *)&address,
                                 (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf("reading");
        valread = read( sock_send , buffer, 1024);
        printf("%s\n",buffer );
    }

    ~Server() {
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }


    sockaddr_in create_sockaddr(String* ip_address, size_t port) {
        struct sockaddr_in our_sockaddr;

        our_sockaddr.sin_family = AF_INET;
        our_sockaddr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, ip_address->cstr_, &our_sockaddr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_address->cstr_);
            printf("\n");
        }

        return our_sockaddr;
    }


    void send_dir_all_clients() {

        Directory* d;

        // spot 0 is for this server.
        for (size_t i =1; i < this->nodes; i++) {
            d = new Directory(0, i, this->nodes, this->ports, this->addresses);
            send_data(*d);
        }
    }


    void send_data(Message m) {

        // create socket
        if ((sock_send = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");

        }

        // connect socket
        struct sockaddr_in our_sockaddr;

        // if message does not have a sockaddr, build one.
        if (m.kind_ == MsgKind::Register) {
            our_sockaddr = create_sockaddr(this->ip_addr, this->port);
        } else {
            String *ip = addresses[m.target_];
            size_t port = ports[m.target_];
            our_sockaddr = create_sockaddr(ip, port);
        }


        if (connect(sock_send, (struct sockaddr *)&our_sockaddr, sizeof(our_sockaddr)) < 0) {
            printf("\nConnection Failed \n");

        }

        // send data
        String* serial = m.serialize();
        send(sock_send , serial, sizeof(serial) , 0 );

        // close socket
        close(sock_send);

    }

    // adds the new node to the addresses and ports fields
    void handle_register(Register r) {
        this->addresses[this->nodes] = r.address;
        this->ports[this->nodes] = r.port;
        nodes++;
    }

    void handle_packet(char* buffer) {
        char** args = new char*[1000];
        int i = 0;
        while (buffer != NULL)
        {
            buffer = strtok (NULL, " ");
            args[i] = buffer;
            i++;
        }
        switch (atoi(args[0])) {
            case 1:
                //Add new ip and port to list
                handle_register(*new Register(buffer));
                //send out new directory to all nodes
                send_dir_all_clients();
                break;
            case 2:

                break;
            case 3:

                break;
            case 4:
                // TODO because server does not receive Directory messages
                break;
        }
    }

    /**
     * Initializes a socket
     */
    int init(const char* ip_address, int port) {
        struct sockaddr_in server_addr;
        char buffer[1024] = {0};
        printf("In client\n");
        printf("creating socket\n");
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            return -1;
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, ip_address, &server_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_addr);
            printf("\n");
            return -1;
        }
        return tmpSocket;
    }
};