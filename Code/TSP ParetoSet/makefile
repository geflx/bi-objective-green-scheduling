output: main.o ParetoSet.o Solution.o
	g++ main.o ParetoSet.o Solution.o -O3 -fopenmp

main.o: main.cpp
	g++ -c main.cpp -O3 -fopenmp

ParetoSet.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -O3 -fopenmp

Solution.o: Solution.cpp Solution.h
	g++ -c Solution.cpp -O3 -fopenmp

clean:
	rm *.o
	rm *.out

test: mainTest.o ParetoSetTest.o SolutionTest.o
	g++ main.o ParetoSet.o Solution.o -g && valgrind --leak-check=full ./a.out

mainTest.o: main.cpp
	g++ -c main.cpp -g

ParetoSetTest.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -g

SolutionTest.o: Solution.cpp Solution.h
	g++ -c Solution.cpp -g
