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
    size_t *ports;  // owned
    String **addresses;  // owned; strings owned

    //Personal Stuff
    String *ip_addr;
    int port;

    //socket stuff
    int sock_listen;
    int sock_send;

    int* sock_send_array;
    int sock_pos = 0;
    sockaddr_in* s;
    int sockaddr_pos = 0;

    Server(String *ip_addr, size_t port) {
        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String *[1000];
        addresses[0] = this->ip_addr;
        this->ports = new size_t[1000];
        ports[0] = this->port;
        this->nodes = 1;
        this->sock_send_array = new int[10000];
        this->s = new sockaddr_in[10000];

        int valread;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);

        printf("Starting server\n");
        printf("Creating Socket\n");

        while (1) {
            // Creating socket file descriptor
            if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("socket failed");
                exit(EXIT_FAILURE);
            }

            printf("setsockopt\n");
            // Forcefully attaching socket to the port 8080
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

            if (inet_pton(AF_INET, "127.0.0.7", &address.sin_addr) <= 0) {
                printf("SERVER: ERROR INET");
                exit(EXIT_FAILURE);
            }

            printf("Binding server socket:");
            printf("%s\n", this->ip_addr->cstr_);
            // Forcefully attaching socket to the port 8080
            if (bind(sock_listen, (struct sockaddr *) &address, sizeof(address)) < 0) {
                int e = errno;
                printf("EXITING %d", e);
                exit(EXIT_FAILURE);
                perror("binding");
            }

            printf("Listening\n");
            if (listen(sock_listen, 3) < 0) {
                perror("listen");
                exit(EXIT_FAILURE);
            }

            printf("Accepting\n");
            if ((sock_send = accept(sock_listen, (struct sockaddr *) &address,
                                    (socklen_t * ) & addrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            sock_send_array[sock_pos] = sock_send;
            sock_pos++;
            s[sockaddr_pos] = address;
            sockaddr_pos++;

            printf("reading\n");

            handle_packet();

            close(sock_listen);
            //close(sock_send);
        }

    }

    ~Server() {
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }


    sockaddr_in create_sockaddr(String *ip_address, size_t port) {
        struct sockaddr_in our_sockaddr;

        our_sockaddr.sin_family = AF_INET;
        our_sockaddr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip_address->cstr_, &our_sockaddr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_address->cstr_);
            printf("\n");
        }

        return our_sockaddr;
    }


    void send_dir_all_clients() {

        Directory *d;

        // spot 0 is for this server.
        for (size_t i = 1; i < this->nodes; i++) {
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
        //if (m.kind_ == MsgKind::Register) {
            our_sockaddr = create_sockaddr(this->ip_addr, this->port);
//        } else {
//            String *ip = addresses[m.target_];
//            size_t port = ports[m.target_];
//            our_sockaddr = create_sockaddr(ip, port);
//        }

        if (connect(sock_send, (struct sockaddr *) &our_sockaddr, sizeof(our_sockaddr)) < 0) {
            printf("\nConnection Failed \n");

        }

        // send data
        String *serial = m.serialize();
        send(sock_send, serial, sizeof(serial), 0);

        // close socket
        //close(sock_send);

    }

    // adds the new node to the addresses and ports fields
    void handle_register(Register r) {
        this->addresses[this->nodes] = r.address;
        this->ports[this->nodes] = r.port;
        nodes++;
        cout << "SERVER: register handled." << endl;
    }

    void handle_packet() {
        printf("SERVER: in handle packet\n");
        char *buffer = new char[10000];
        read(sock_send, buffer, 10000);

        char* msg_kind = new char[10000];
        *msg_kind = buffer[0];
        //cout << buffer << endl;

        // check message kind
        switch (atoi(msg_kind)) {
            case 1:
                cout << "SERVER: handling register message" << endl;
                //Add new ip and port to list
                handle_register(*new Register(buffer));

                cout << "nodes: ";
                cout << nodes << endl;

                for (size_t i = 1; i < this->nodes; i++) {
                    cout << addresses[i]->cstr_ << endl;
                    Directory *d = new Directory(0, i, this->nodes, this->ports, this->addresses);

                    if ((sock_send_array[i - 1] = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                        printf("\n Socket creation error \n");
                    }
                    if (connect(sock_send_array[i - 1], (struct sockaddr *) &s[i-1], sizeof(s[i-1])) < 0) {
                        printf("\nConnection Failed \n");
                    }
                    cout << d->serialize()->cstr_ << endl;
                    send(sock_send_array[i - 1], d->serialize()->cstr_, 10000, 0);
                    //send_data(d);
                    cout << d->serialize()->cstr_ << endl;
                }

                break;
            case 2: // ack
                cout << "Ack recieved" << endl;
                break;
            case 3: //status
                cout << "stats" << endl;
                handle_status(*new Status(buffer));

                break;
            case 4:
                // TODO because server does not receive Directory messages
                break;
        }
    }

    void handle_status(Status s) {
        cout << s.msg_->cstr_ << endl;
        Ack* ack = new Ack(0, s.sender_);
        send(sock_send, ack->serialize()->cstr_, 10000, 0);
    }

    /**
     * Initializes a socket
     */
    int init(const char *ip_address, int port) {
        struct sockaddr_in server_addr;
        char buffer[1024] = {0};
        printf("In client\n");
        printf("creating socket\n");
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip_address, &server_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_addr);
            printf("\n");
            return -1;
        }
        return tmpSocket;
    }
};