CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = satnet.o

mytest: $(OBJECTS) mytest.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) mytest.cpp -o mytest

satnet.o: satnet.h satnet.cpp
	$(CXX) $(CXXFLAGS) -c satnet.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./mytest

val:
	valgrind ./mytest

turnin: 
	cp satnet.h satnet.cpp mytest.cpp ~/cs341proj/proj2/

check: 
	ls ~/cs341proj/proj2