#include "serialize.h"
//#include "list.h"
#include "string.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#pragma once

class Node {
public:
    //FIRST NODE IS THE SERVER
    size_t nodes;
    size_t * ports;  // owned
    String ** addresses;  // owned; strings owned

    //Personal Stuff
    String* server_addr;
    String* ip_addr;
    size_t port;
    size_t server_port;

    int sock_listen;
    int sock_send;

     Node(String* server_addr, size_t server_port, String* ip_addr, size_t port) {
        this->server_addr = server_addr;
        this->server_port = server_port;

        this->ip_addr = ip_addr;
        this->port = port;

        this->addresses = new String*[1000];
        addresses[0] = this->server_addr;
        this->ports = new size_t[1000];
        ports[0] = this->server_port;
        this->nodes = 1;

        this->sock_listen = init(this->ip_addr->cstr_, this->port);

        this->send_reg();
    }

    ~Node() {
        delete server_addr;
        delete ip_addr;
        delete[] ports;
        delete[] addresses;
        close(sock_listen);
        close(sock_send);
    }

    void send_reg() {
       send_data(*new Register(-1, 0, this->port, this->ip_addr));
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

     void send_data(Message m) {

         // create socket
         if ((sock_send = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
             printf("\n Socket creation error \n");

         }

         // connect socket
         struct sockaddr_in our_sockaddr;

         // if message does not have a sockaddr, build one.
         if (m.kind_ == MsgKind::Register) {
             our_sockaddr = create_sockaddr(this->server_addr, this->server_port);
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

     void handle_directory(Directory* d) {
         this->ports = d->ports;
         this->addresses = d->addresses;
         this->nodes = d->nodes;
     }

     // Receives type Message -- has MsgKind field
     void handle_packet(char* buffer) {
        switch (buffer[0]) {
            case 1:
                //TODO error?
                break;
            case 2:
                //TODO
                break;
            case 3:

                break;
            case 4: //Directory
                Directory* d = new Directory(buffer);
                handle_directory(d);
                break;
        }
     }

     String* get_address() {
         return this->ip_addr;
     }

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
         if (connect(tmpSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
         {
             printf("\nConnection Failed \n");
             return -1;
         }
         return tmpSocket;
     }
};