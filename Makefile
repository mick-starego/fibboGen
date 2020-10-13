CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -g -Wall -O2 -fsanitize=leak,address

OBJECTS = FibboGen.o Hypers.o timer.o

fibboGen: $(OBJECTS)
	$(CXX) $(OBJECTS) -o FibboGen

.PHONY: clean
clean:
	-rm -f  *.o *~ FibboGen *.ts

FibboGen.o: FibboGen.cpp

Hypers.o: Hypers.cpp

timer.o: timer.cpp

