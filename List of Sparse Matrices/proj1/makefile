CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = csr.o

mytest: $(OBJECTS) mytest.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) mytest.cpp -o mytest

csr.o: csr.h csr.cpp
	$(CXX) $(CXXFLAGS) -c csr.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./mytest

val:
	valgrind ./mytest

turnin: 
	   cp csr.h csr.cpp mytest.cpp ~/cs341proj/proj1/

check: 
	ls ~/cs341proj/proj1