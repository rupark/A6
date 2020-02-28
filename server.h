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

//        int valread;
//        struct sockaddr_in address;
//        int opt = 1;
//        int addrlen = sizeof(address);
//
//
//
//        printf("Starting server\n");
//        printf("Creating Socket\n");
//
//        // Creating socket file descriptor
//        if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) == 0)
//        {
//            perror("socket failed");
//            exit(EXIT_FAILURE);
//        }
//
//        printf("setsockopt\n");
//        // Forcefully attaching socket to the port 8080
//        if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEADDR,
//                       &opt, sizeof(opt)) < 0)
//        {
//            perror("setsockopt");
//            exit(EXIT_FAILURE);
//        }
//
//        if (setsockopt(sock_listen, SOL_SOCKET, SO_REUSEPORT,
//                       &opt, sizeof(opt)) < 0)
//        {
//            perror("setsockopt");
//            exit(EXIT_FAILURE);
//        }
//
//        address.sin_family = AF_INET;
//        address.sin_port = htons(this->port);
//
//        if(inet_pton(AF_INET,"127.0.0.5", &address.sin_addr) <= 0) {
//            printf("SERVER: ERROR INET");
//            exit(EXIT_FAILURE);
//        }
//
//        printf("Binding server socket:");
//        printf("%s\n", this->ip_addr->cstr_);
//        // Forcefully attaching socket to the port 8080
//        if (bind(sock_listen, (struct sockaddr *)&address, sizeof(address))<0)
//        {
//            int e = errno;
//            printf("EXITING %d", e);
//            exit(EXIT_FAILURE);
//            perror("binding");
//        }
//
//        printf("Listening\n");
//        if (listen(sock_listen, 3) < 0)
//        {
//            perror("listen");
//            exit(EXIT_FAILURE);
//        }
//
//        printf("Accepting\n");
//        if ((sock_send = accept(sock_listen, (struct sockaddr *)&address,
//                                 (socklen_t*)&addrlen))<0)
//        {
//            perror("accept");
//            exit(EXIT_FAILURE);
//        }
//        printf("reading\n");
//       // valread = read( sock_send , buffer, 100000);
//        //printf("%s\n",buffer);

        int opt = 1;
        int master_socket , addrlen , new_socket , client_socket[30] ,
                max_clients = 30 , activity, i , valread , sd;
        int max_sd;
        struct sockaddr_in address;

        char buffer[1025];  //data buffer of 1K

        //set of socket descriptors
        fd_set readfds;

        //initialise all client_socket[] to 0 so not checked
        for (i = 0; i < max_clients; i++)
        {
            client_socket[i] = 0;
        }

        //Creating socket file descriptor
        if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        printf("setsockopt\n");
        // Forcefully attaching socket to the port 8080
        if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR,
                       &opt, sizeof(opt)) < 0)
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEPORT,
                       &opt, sizeof(opt)) < 0)
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(this->port);

        if(inet_pton(AF_INET,"127.0.0.3", &address.sin_addr) <= 0) {
            printf("SERVER: ERROR INET");
            exit(EXIT_FAILURE);
        }

        printf("Binding server socket:");
        printf("%s\n", this->ip_addr->cstr_);
        // Forcefully attaching socket to the port 8080
        if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)
        {
            int e = errno;
            printf("EXITING %d", e);
            exit(EXIT_FAILURE);
            perror("binding");
        }

        printf("Listening\n");
        if (listen(master_socket, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }

        //accept the incoming connection
        addrlen = sizeof(address);
        puts("Waiting for connections ...");

        while(1)
        {
            //clear the socket set
            FD_ZERO(&readfds);

            //add master socket to set
            FD_SET(master_socket, &readfds);
            max_sd = master_socket;

            //add child sockets to set
            for ( i = 0 ; i < max_clients ; i++)
            {
                //socket descriptor
                sd = client_socket[i];

                //if valid socket descriptor then add to read list
                if(sd > 0)
                    FD_SET( sd , &readfds);

                //highest file descriptor number, need it for the select function
                if(sd > max_sd)
                    max_sd = sd;
            }

            //wait for an activity on one of the sockets , timeout is NULL ,
            //so wait indefinitely
            activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

            if ((activity < 0) && (errno!=EINTR))
            {
                printf("select error");
            }

            //If something happened on the master socket ,
            //then its an incoming connection
            if (FD_ISSET(master_socket, &readfds))
            {
                if ((new_socket = accept(master_socket,
                                         (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

                handle_packet();

//                //send new connection greeting message
//                if( send(new_socket, message, strlen(message), 0) != strlen(message) )
//                {
//                    perror("send");
//                }

                puts("Welcome message sent successfully");

                //add new socket to array of sockets
                for (i = 0; i < max_clients; i++)
                {
                    //if position is empty
                    if( client_socket[i] == 0 )
                    {
                        client_socket[i] = new_socket;
                        printf("Adding to list of sockets as %d\n" , i);

                        break;
                    }
                }
            }

//            //else its some IO operation on some other socket
//            for (i = 0; i < max_clients; i++)
//            {
//                sd = client_socket[i];
//
//                if (FD_ISSET( sd , &readfds))
//                {
//                    //Check if it was for closing , and also read the
//                    //incoming message
//                    if ((valread = read( sd , buffer, 1024)) == 0)
//                    {
//                        //Somebody disconnected , get his details and print
//                        getpeername(sd , (struct sockaddr*)&address , \
//                        (socklen_t*)&addrlen);
//                        printf("Host disconnected , ip %s , port %d \n" ,
//                               inet_ntoa(address.sin_addr) , ntohs(address.sin_port));
//
//                        //Close the socket and mark as 0 in list for reuse
//                        close( sd );
//                        client_socket[i] = 0;
//                    }
//
//                        //Echo back the message that came in
//                    else
//                    {
//                        //set the string terminating NULL byte on the end
//                        //of the data read
//                        buffer[valread] = '\0';
//                        send(sd , buffer , strlen(buffer) , 0 );
//                    }
//                }
//            }
        }

        handle_packet();
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
        cout << "SERVER: register handled." << endl;
    }

    void handle_packet() {
        printf("SERVER: in handle packet\n");
        char* buffer = new char[10000];
        read( sock_send , buffer, 10000);
//
//        char** args = new char*[1000];
//        char* token = strtok(buffer, "?");
//        int i = 0;
//
//        while (token != NULL)
//        {
//            args[i] = token;
//            token = strtok (NULL, "?");
//            printf("TOKENS: %s\n", args[i]);
//            i++;
//        }

        char* msg_kind = &buffer[0];

        // check message kind
        switch (atoi(msg_kind)) {
            case 1:
                cout << "SERVER: handling register message" << endl;
                //Add new ip and port to list
                handle_register(*new Register(buffer));

                cout << "address:";
                cout << addresses[1]->cstr_ << endl;

                cout << "nodes: ";
                cout << nodes << endl;

                //send out new directory to all nodes
                //send_dir_all_clients();
                break;
            case 2:

                break;
            case 3: //status
                //cout << args[3] << endl;
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