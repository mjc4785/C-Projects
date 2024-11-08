CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = pqueue.o

mytest: $(OBJECTS) mytest.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) mytest.cpp -o mytest

pqueue.o: pqueue.h pqueue.cpp
	$(CXX) $(CXXFLAGS) -c pqueue.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./mytest

val:
	valgrind ./mytest

turnin: 
	cp pqueue.h pqueue.cpp mytest.cpp ~/cs341proj/proj3/

check: 
	ls ~/cs341proj/proj3