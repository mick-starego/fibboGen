CXX = clang++ $(CXXFLAGS)
CXXFLAGS = -g -Wall -O2 -fsanitize=leak,address

OBJECTS = FibboGen.o TreeNode.o

fibboGen: $(OBJECTS)
	$(CXX) $(OBJECTS) -o FibboGen

.PHONY: clean
clean:
	-rm -f  *.o *~ FibboGen

FibboGen.o: FibboGen.cpp

TreeNode.o: TreeNode.cpp

