#ifndef A5EXPERIMENTS_NETWORK_H
#define A5EXPERIMENTS_NETWORK_H

#endif //A5EXPERIMENTS_NETWORK_H

#pragma once

#include "server.h"
#include "node.h"

//A Network consists of a Server and a list of Node
class Network {
public:
    Server *server;
    Node** nodes;

    Network(Server *server, Node** n, int num) {
        this->server = server;
        for (int i = 0; i < num; i++) {
            this->nodes[i] = n[i];
        }
    }
};