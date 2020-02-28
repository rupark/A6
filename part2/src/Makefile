test: string.h object.h helper.h serial.h test.o
	g++ --std=c++11 string.h object.h helper.h serial.h test.o -o test

test.o:
	g++ --std=c++11 -c main.cpp -o test.o

build: test

run:
	./test

clean:
	rm test *.o *.h.gch