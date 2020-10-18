CFLAGS=-fopenmp
CXX=g++

all: pi hello

clean:
	rm -f pi hello mandel

pi:
	$(CXX) $(CFLAGS) pi.cpp -o pi

hello:
	$(CXX) $(CFLAGS) hello.cpp -o hello

mandel:
	$(CXX) $(CFLAGS) mandel.cpp -o mandel

