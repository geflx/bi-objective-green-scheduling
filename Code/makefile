output: main.o ParetoSet.o Solution.o Laboratory.o
	g++ main.o ParetoSet.o Solution.o Laboratory.o -O3

main.o: main.cpp
	g++ -c main.cpp -O3

ParetoSet.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -O3

Solution.o: Solution.cpp Solution.h
	g++ -c Solution.cpp -O3

Laboratory.o: Laboratory.cpp Laboratory.h
	g++ -c Laboratory.cpp -O3

clean:
	rm *.o output