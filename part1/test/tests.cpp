#include <stdio.h>
#include <iostream>
#include "../src/serialize.h"
#include "../src/node.h"
#include "../src/server.h"

using namespace std;

int main(int argc, char const *argv[])
{
    Server* s1 = new Server(new String("127.0.0.3"), 8080);

    cout << "Creating Node" << endl;
    Node* n1 = new Node(new String("127.0.0.3"),8080, new String("127.0.0.1"), 8080);
    cout << "Node Created" << endl;

    // DUMPING TABLES
    for (size_t j = 0; j < s1->nodes; j++) {
        cout << "SERVER: ";
        printf("%s\n",s1->addresses[j]->c_str());

        cout << "NODE: ";
        printf("%s\n",n1->addresses[j]->c_str());
    }

}