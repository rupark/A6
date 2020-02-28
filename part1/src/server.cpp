#include <stdio.h>
#include <iostream>
#include "serialize.h"
#include "node.h"
#include "server.h"

using namespace std;

int main(int argc, char const *argv[])
{
    cout << "Creating Server" << endl;
    Server* s1 = new Server(new String("127.0.0.7"), 8080);
    cout << "Server Made" << endl;

}