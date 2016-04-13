CXX = clang++
CXXFLAGS = -ggdb -Wall -Wextra -Wno-gnu-case-range $(shell pkg-config --cflags fuzzylite)
LDFLAGS = -lfuzzylite-static #$(shell pkg-config --libs fuzzylite)
OBJS = example.o

safety-rating.run: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o safety-rating.run $(LDFLAGS)

