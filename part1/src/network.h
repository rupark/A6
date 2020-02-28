#ifndef A5EXPERIMENTS_NETWORK_H
#define A5EXPERIMENTS_NETWORK_H

#endif //A5EXPERIMENTS_NETWORK_H

#pragma once

#include "server.h"
#include "list.h"

class Network {
public:
    // server
    Server *server;

    // Cannot be a network without Registration Server.
    Network(Server *server) {
        this->server = server;
    }

    // Adds Node to network
    add_node(Node* node) {
        // adds to to network
        node->send_reg();
    }



};