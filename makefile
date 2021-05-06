CXX = g++
CXXFLAGS = -std=c++0x -Wall -g -w

P2: main.cpp scanner.cpp parseTree.cpp
	$(CXX) $(CXXFLAGS) -o compfs main.cpp parseTree.cpp scanner.cpp semantics.cpp

clean:
	rm -f compfs *.o stdin.temp kb.asm file.asm
