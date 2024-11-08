CXX = g++
CXXFLAGS = -Wall -g
OBJECTS = dealer.o

mytest: $(OBJECTS) mytest.cpp
	$(CXX) $(CXXFLAGS) $(OBJECTS) mytest.cpp -o mytest

dealer.o: dealer.h dealer.cpp
	$(CXX) $(CXXFLAGS) -c dealer.cpp

clean:
	rm *.o*
	rm *~ 

run:
	./mytest

val:
	valgrind ./mytest

turnin: 
	cp dealer.h dealer.cpp mytest.cpp ~/cs341proj/proj4/

check: 
	ls ~/cs341proj/proj4