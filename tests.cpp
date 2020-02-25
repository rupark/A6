#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "string.h"
#include "network.h"
#include "node.h"
#include "server.h"
#include "serialize.hr"

#define PORT 8080

int main(int argc, char const *argv[])
{
    Network* n = new Network(new Server(new String("127.0.0.2")));
    Node* client1 = new Node(new String("127.0.0.1"));
    n.add_node(n);
    return 0;
}