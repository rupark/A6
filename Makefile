build: 
	g++ client.cpp -o client
	g++ server.cpp -o server

run:
	./client && ./server

clean:
	rm client server
