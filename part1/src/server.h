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

#include "string.h"

#pragma once

class Server {
public:
    //Directory
    //Server is index 0
    size_t nodes;
    size_t *ports;
    String **addresses;

    //Personal Stuff
    String *ip_addr;
    int port;

    //Socket stuff
    int sock_listen;
    int sock_send;

    //Array of Node sockets
    int* sock_send_array;
    int sock_pos = 0;

    Server(String *ip_addr, size_t port) {
        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String *[1000];
        addresses[0] = this->ip_addr;
        this->ports = new size_t[1000];
        ports[0] = this->port;
        this->nodes = 1;

        int valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        this->sock_send_array = new int[10000];

        while (1) {
            // Creating socket file descriptor
            if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }
            
            // Forcefully attaching socket to the port 8080, enabling reusing port and address
            if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR,
                           &opt, sizeof(opt)) < 0) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }

            if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEPORT,
                           &opt, sizeof(opt)) < 0) {
                perror("setsockopt");
                exit(EXIT_FAILURE);
            }

            address.sin_family = AF_INET;
            address.sin_port = htons(this->port);

            if (inet_pton(AF_INET, this->ip_addr->cstr_, &address.sin_addr) <= 0) {
                printf("SERVER: ERROR INET");
                exit(EXIT_FAILURE);
            }
            
            // Forcefully attaching socket to the port 8080
            if (bind(sock_listen, (struct sockaddr *) &address, sizeof(address)) < 0) {
                int e = errno;
                printf("EXITING %d", e);
                exit(EXIT_FAILURE);
                perror("binding");
            }

            //Listening for transmission
            printf("SERVER: Listening\n");
            if (listen(sock_listen, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }

            //Accepting connection
            printf("SERVER: Accepting\n");
            if ((sock_send = accept(sock_listen, (struct sockaddr *) &address,
                                    (socklen_t * ) & addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            
            //Adding socket to list of Node sockets
            sock_send_array[sock_pos] = sock_send;
            sock_pos++;

            //Handling recieved data
            handle_packet();

            close(sock_listen);
        }

    }

    ~Server() {
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }

    // adds the new node to the addresses and ports fields
    void handle_register(Register r) {
        this->addresses[this->nodes] = r.address;
        this->ports[this->nodes] = r.port;
        nodes++;
        cout << "SERVER: register handled." << endl;
        
        for (size_t i = 1; i < this->nodes; i++) {
            Directory *d = new Directory(0, i, this->nodes, this->ports, this->addresses);

            this->sock_send = init_client();

            if ((sock_send = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                printf("\n Socket creation error \n");
                exit(1);
            }

            sock_send = sock_send_array[0];

            cout << "SERVER: sending Directory" << endl;
            send(sock_send, d->serialize()->cstr_, 10000, 0);
        }
    }

    //Handles recieved data from socket
    void handle_packet() {
        char *buffer = new char[10000];
        read(sock_send, buffer, 10000);

        char* msg_kind = new char[10000];
        *msg_kind = buffer[0];

        // check message kind
        switch (atoi(msg_kind)) {
            case 1: //Register
                cout << "SERVER: recieved Register" << endl;
                //Add new ip and port to list
                handle_register(*new Register(buffer));
                break;
            case 2: //Ack
                cout << "SERVER: recieved Ack" << endl;
                handle_ack(*new Ack(buffer));
                break;
            case 3: //Status
                cout << "SERVER: recieved Status" << endl;
                handle_status(*new Status(buffer));
                break;
            case 4: //Directory
                //Servers cannot recieve Directory
                exit(1);
                break;
        }
    }

    //Handles Status and returns Ack
    void handle_status(Status s) {
        cout << s.msg_->cstr_ << endl;
        Ack* ack = new Ack(0, s.sender_);
        cout << "SERVER: sending Ack" << endl;
        send(sock_send, ack->serialize()->cstr_, 10000, 0);
    }

    //Handles Ack
    void handle_ack(Ack a) {

    }

    //Initializes listening socket
    int init(const char *ip_address, int port) {
        struct sockaddr_in server_addr;
        char buffer[1024] = {0};
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(1);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            exit(1);
        }
        return tmpSocket;
    }

    //Creates sockaddr
    sockaddr_in create_sockaddr(String* ip_address, size_t port) {
        struct sockaddr_in our_sockaddr;

        our_sockaddr.sin_family = AF_INET;
        our_sockaddr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, ip_address->cstr_, &our_sockaddr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported");
            exit(1);
        }

        return our_sockaddr;
    }

    //Used for initializing sock_send when interacting with client sockets
    int init_client() {
        struct sockaddr_in server_addr;
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Socket creation error \n");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if(inet_pton(AF_INET, this->ip_addr->cstr_, &server_addr.sin_addr)<=0)
        {
            printf("\nInvalid address/ Address not supported");
            exit(1);
        }

        if (connect(tmpSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            printf("\nConnection Failed \n");
            exit(1);
        }
        return tmpSocket;
    }
};