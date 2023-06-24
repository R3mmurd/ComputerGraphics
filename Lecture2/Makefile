CXX = clang++ -std=c++14

LIBS = -lGL -lGLEW -lglfw

all: uniform_variables translation rotation scale combining

uniform_variables: uniform_variables.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

translation: translation.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

rotation: rotation.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

scale: scale.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

combining: combining.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

.PHONY:
clean:
	$(RM) uniform_variables translation rotation scale combining
