output: main.o ParetoSet.o
	g++ main.o ParetoSet.o -O3

main.o: main.cpp
	g++ -c main.cpp -O3

ParetoSet.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -O3

clean:
	rm *.o output