main: main.o line.o ssa.o optimization.o
	g++ -std=c++11 -Wall -Wno-write-strings -o $@ $^ -lm

main.o: main.cpp ssa.hpp line.hpp
	g++ -std=c++11 -Wall -Wno-write-strings -c $<

ssa.o: ssa.cpp ssa.hpp line.hpp
	g++ -std=c++11 -Wall -Wno-write-strings -c $<

optimization.o: optimization.cpp optimization.hpp line.hpp
	g++ -std=c++11 -Wall -Wno-write-strings -c $<

line.o: line.cpp line.hpp
	g++ -std=c++11 -Wall -Wno-write-strings -c $<
