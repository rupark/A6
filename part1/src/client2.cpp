#include <stdio.h>
#include <iostream>
#include "serialize.h"
#include "node.h"
#include "server.h"

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Creating Node" << endl;
    Node* n2 = new Node(new String("127.0.0.7"),8080, new String("127.0.0.2"), 8080);
    cout << "Node Created" << endl;

}