#build: serialize.h server.h node.h test.o
#	g++ --std=c++11 node.h serialize.h server.h node.h test.o

client: serialize.h server.h node.h testClient.o
	g++ --std=c++11 node.h serialize.h server.h testClient.o node.h -o client

server: serialize.h server.h node.h testServer.o
	g++ --std=c++11 node.h serialize.h server.h testServer.o node.h -o server

client2: serialize.h server.h node.h testClient2.o
        g++ --std=c++11 node.h serialize.h server.h testClient2.o node.h -o client

test.o:
	g++ --std=c++11 -c tests.cpp -o test.o

testClient2.o:
    g++ --std=c++11 -c client2.cpp -o testClient2.o

testClient.o:
	g++ --std=c++11 -c testClient.cpp -o testClient.o

testServer.o:
	g++ --std=c++11 -c testServer.cpp -o testServer.o

build: client server


run:
	./server & ./client

clean:
	rm *.out *.o *.h.gch client server
