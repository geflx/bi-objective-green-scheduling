output: main.o ParetoSet.o Solution.o Laboratory.o
	g++ main.o ParetoSet.o Solution.o Laboratory.o -O3 -fopenmp

main.o: main.cpp
	g++ -c main.cpp -O3 -fopenmp

ParetoSet.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -O3 -fopenmp

Solution.o: Solution.cpp Solution.h
	g++ -c Solution.cpp -O3 -fopenmp

Laboratory.o: Laboratory.cpp Laboratory.h
	g++ -c Laboratory.cpp -O3 -fopenmp

clean:
	rm *.o output