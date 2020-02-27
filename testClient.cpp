#include <stdio.h>
#include <iostream>
#include "serialize.h"
#include "node.h"
#include "server.h"

using namespace std;

int main(int argc, char const *argv[])
{
    //Status* s = new Status(0, 1, new String("hello"));

//    size_t* ports = new size_t[3];
//    ports[0] = 1;
//    ports[1] = 1;
//    ports[2] = 1;
//    String** add = new String*[3];
//    add[0] = new String("127.0.0.1");
//
//    cout << "Creating Server" << endl;
//    Server* s1 = new Server(new String("127.0.0.2"), 8080);
//    cout << "Server Made" << endl;

    cout << "Creating Node" << endl;
    Node* n1 = new Node(new String("127.0.0.2"),8080, new String("127.0.0.1"), 8080);
    cout << "Node Created" << endl;

//    // DUMPING TABLES
//    for (size_t j = 0; j < s1->nodes; j++) {
//        cout << "SERVER: ";
//        printf("%s\n",s1->addresses[j]->c_str());
//
//        cout << "NODE: ";
//        printf("%s\n",n1->addresses[j]->c_str());
//    }




//    Directory* d = new Directory(0, 1, 3, ports, add);





    //Node* n = new Node(new String("h"), 10, new String("l"), 11);
    //n->handle_packet(s->serialize()->cstr_);
    //cout << s->serialize()->cstr_ << endl;
//    cout << d->serialize()->cstr_ << endl;
//    char* buffer = d->serialize()->c_str();
//    Directory* e = new Directory(buffer);
//    cout << e->serialize()->cstr_ << endl;





}