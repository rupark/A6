//
// Created by kramt on 2/21/2020.
//
#include "list.h"
#include "node.h"
#include "serialize.h"
#include "IP_Port_Tuple.h"
#include "string.h"

class Server {
public:

    // Know its own ip address (master)
    String* ip_addr;

    // know all the other ip address of clients
    // array list of nodes
    IPList* ip_list;

    Server(String* ip_addr) {
        this->ip_addr = ip_addr;
        ip_list = new IPList();
    }

    ~Server() {
        delete nodes;
        delete ip_addr;
    }

    void handle_packet(Message* m) {

    }

    void send_update() {

    }

    void send_data(Message* m, IP_Port_Tuple* ipNport) {

    }

    // returns master address (this address)
    virtual String* get_master_addr() {
        return this->ip_addr;
    }

    // returns all the addresses of the clients saved in the address table
    virtual IPList* dump_table() {
        return this->ip_list;
    }

};