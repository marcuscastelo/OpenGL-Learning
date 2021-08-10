
CXX=g++
CPP_STANDARD=c++17
CXXFLAGS=-std=${CPP_STANDARD} -g -ggdb -O0
LDFLAGS=-Ldependencies/glfw/build/src -Idependencies/glew/lib
LDLIBS=-pthread -ldl -lglfw3 -lGLEW -lGL  
INCLUDES=-Idependencies/glfw/include -Idependencies/glew/include

all: main

run: main
	./bin/main

.PHONY: main
main: bin/main

bin/main: src/main.cpp src/Shader.cpp src/vendor/stb_image/stb_image.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS) $(INCLUDES)  
