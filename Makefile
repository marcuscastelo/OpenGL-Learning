
CXX=g++
CPP_STANDARD=c++17
CXXFLAGS=-std=${CPP_STANDARD}
LDFLAGS=-Ldependencies/glfw/build/src
LDLIBS=-pthread -ldl -lglfw3
INCLUDES=-Idependencies/glfw/include

all: main

run: main
	./bin/main

.PHONY: main
main: bin/main

bin/main: main.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS) $(INCLUDES)  
