CXX = clang++ -std=c++14

LIBS = -lGL -lGLEW -lglfw

all: main

main: main.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

.PHONY:
clean:
	$(RM) main
