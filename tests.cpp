// EXAMPLE OF CLIENT
// SOURCE: https://www.geeksforgeeks.org/socket-programming-cc/

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
    Network n = new Network(new Server(new String("127.0.0.2")));
    Node client1 = new Node(new String("127.0.0.1"));


    return 0;
}