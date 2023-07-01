CXX = clang++ -std=c++14

LIBS = -lGL -lGLEW -lglfw

all: interpolation indexed_draw projections

interpolation: interpolation.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

indexed_draw: indexed_draw.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

projections: projections.cpp
	$(CXX) $@.cpp -o $@ $(LIBS)

.PHONY:
clean:
	$(RM) interpolation indexed_draw projections
