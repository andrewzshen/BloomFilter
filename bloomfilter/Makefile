CXX = g++
CXXFLAGS = -std=c++17

main: Main.o BloomFilter.o HashFunctions.o
	${CXX} ${CXXFLAGS} $^ -o $@ 

clean:
	rm -r Main *.o