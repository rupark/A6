build: serialize.h server.h node.h test.o
	g++ --std=c++11 node.h serialize.h server.h node.h test.o 

build_client: serialize.h server.h node.h testClient.o
	g++ --std=c++11 node.h serialize.h server.h node.h testClient.o

build_server: serialize.h server.h node.h testServer.o
	g++ --std=c++11 node.h serialize.h server.h node.h testServer.o

test.o:
	g++ --std=c++11 -c tests.cpp

testClient.o:
	g++ --std=c++11 -c testClient.cpp

testServer.o:
	g++ --std=c++11 -c testServer.cpp

run:
	./server && ./client

clean:
	rm a.out test.o *.h.gch
