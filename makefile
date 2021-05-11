CXX = g++
CXXFLAGS = -std=c++0x -Wall -g -w

P4: main.cpp scanner.cpp parseTree.cpp
	$(CXX) $(CXXFLAGS) -o compfs main.cpp parseTree.cpp scanner.cpp semantics.cpp

clean:
	rm -f compfs *.o stdin.temp *.asm
