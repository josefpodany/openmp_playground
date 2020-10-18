CFLAGS=-fopenmp
CXX=g++

all: pi hello

clean:
	rm pi hello

pi:
	$(CXX) $(CFLAGS) pi.cpp -o pi

hello:
	$(CXX) $(CFLAGS) hello.cpp -o hello
