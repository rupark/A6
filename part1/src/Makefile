client: serialize.h server.h node.h testClient.o
	g++ --std=c++11 node.h serialize.h server.h testClient.o node.h -o client

server: serialize.h server.h node.h testServer.o
	g++ --std=c++11 node.h serialize.h server.h testServer.o node.h -o server

testClient.o:
	g++ --std=c++11 -c client.cpp -o testClient.o

testServer.o:
	g++ --std=c++11 -c server.cpp -o testServer.o

build: client server

run:
	./server & ./client

clean:
	rm *.out *.o *.h.gch client server
