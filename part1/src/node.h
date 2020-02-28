#include "serialize.h"
#include "string.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "string.h"
#include <arpa/inet.h>
#include <assert.h>

#pragma once

class Node {
public:
    //Network Directory
    //Index 0 is the Server info
    size_t nodes;
    size_t *ports;
    String **addresses;

    //Personal Stuff
    String *server_addr;
    String *ip_addr;
    size_t port;
    size_t server_port;

    //Sockets
    int sock_listen;
    int sock_send;

    Node(String *server_addr, size_t server_port, String *ip_addr, size_t port) {
        close(sock_send);
        this->server_addr = server_addr;
        this->server_port = server_port;
        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String *[1000];
        addresses[0] = this->server_addr;
        this->ports = new size_t[1000];
        ports[0] = this->server_port;
        this->nodes = 1;

        this->sock_send = init_client();

        printf("NODE: registering with server");
        this->send_reg();
        printf("NODE: done registering");
        close(sock_send);

        this->sock_send = init_client();
        printf("NODE: sending status");
        this->send_status();
        printf("NODE: done sending status");

        while (1) {
            handle_packet();
        }
    }

    ~Node() {
        delete server_addr;
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }

    //Registers with Server
    //From index is -1 since we do not yet know this Node's index
    void send_reg() {
        send_data(new Register(-1, 0, this->port, this->ip_addr));
    }

    //Sends status to Server
    void send_status() {
        send_data(new Status(-1, 0, new String("Hello from Node!")));
    }

    //Creates a sockaddr
    sockaddr_in create_sockaddr(String *ip_address, size_t port) {
        struct sockaddr_in our_sockaddr;

        our_sockaddr.sin_family = AF_INET;
        our_sockaddr.sin_port = htons(port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, ip_address->cstr_, &our_sockaddr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            exit(1);
        }

        return our_sockaddr;
    }

    //Sends the given Message
    void send_data(Message *m) {

        // create socket
        if ((sock_send = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            exit(1);
        }

        // connect socket
        struct sockaddr_in our_sockaddr;

        our_sockaddr = create_sockaddr(this->server_addr, this->server_port);

        if (connect(sock_send, (struct sockaddr *) &our_sockaddr, sizeof(our_sockaddr)) < 0) {
            printf("\nConnection Failed \n");
            exit(1);
        }

        String *serial;

        // Serializing the Message
        // Nodes cannot send Directory Messages
        if (m->kind_ == MsgKind::Register) {
            serial = dynamic_cast<Register *>(m)->serialize();
        } else if (m->kind_ == MsgKind::Status) {
            serial = dynamic_cast<Status *>(m)->serialize();
            cout << serial->cstr_ << endl;
        } else {
            serial = dynamic_cast<Ack *>(m)->serialize();
        }

        send(sock_send, serial->cstr_, 10000, 0);

        handle_packet();

    }

    //Handles Directory; Sets this Node's fields to the given Directory's
    void handle_directory(Directory *d) {
        this->ports = d->ports;
        this->addresses = d->addresses;
        this->nodes = d->nodes;
    }

    //Handles Ack
    void handle_ack(Ack *a) {

    }

    //Handles Status; prints the Status' message
    void handle_status(Status *s) {
        printf(s->msg_->cstr_);
    }

    //Reads incoming data and parses it into a Message and responds appropriately
    void handle_packet() {
        char *buffer = new char[10000];
        read(sock_send, buffer, 10000);
        char *msg_kind = new char[10000];
        *msg_kind = buffer[0];

        switch (atoi(msg_kind)) {
            case 1: //Register
                //ERROR: Cannot Register with a Node
                exit(1);
                break;
            case 2: //Ack
            {
                printf("Ack recieved in NODE");
                Ack *a = new Ack(buffer);
                handle_ack(a);
                break;
            }
            case 3: //Status
            {
                printf("Directory recieved in NODE");
                Status *s = new Status(buffer);
                handle_status(s);
                break;
            }
            case 4: //Directory
            {
                printf("Directory recieved in NODE");
                Directory *d = new Directory(buffer);
                handle_directory(d);
                break;
            }
        }
    }

    //Used for initializing sock_send when sending to nodes
    int init_client() {
        struct sockaddr_in server_addr;
        //char buffer[1024] = {0};
        printf("In client\n");
        printf("creating socket\n");
        int tmpSocket = 0;
        if ((tmpSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Socket creation error \n");
            return -1;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(this->port);

        // Convert IPv4 and IPv6 addresses from text to binary form
        if (inet_pton(AF_INET, this->ip_addr->cstr_, &server_addr.sin_addr) <= 0) {
            printf("\nInvalid address/ Address not supported");
            printf("%s", ip_addr);
            printf("\n");
            return -1;
        }

        if (connect(tmpSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
            printf("\nConnection Failed \n");
            return -1;
        }
        return tmpSocket;
    }
};