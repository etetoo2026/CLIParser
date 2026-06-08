CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

all: example/myapp tests/run_tests

example/myapp: example/main.cpp cliparser.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

tests/run_tests: tests/test_parser.cpp cliparser.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: tests/run_tests
	./tests/run_tests

clean:
	rm -f example/myapp tests/run_tests
