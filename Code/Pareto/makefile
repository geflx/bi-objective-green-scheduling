output: main.o ParetoSet.o Solution.o Lab.o
	g++ main.o ParetoSet.o Solution.o Lab.o -O3

main.o: main.cpp
	g++ -c main.cpp -O3

ParetoSet.o: ParetoSet.cpp ParetoSet.h
	g++ -c ParetoSet.cpp -O3

Solution.o: Solution.cpp Solution.h
	g++ -c Solution.cpp -O3

Lab.o: Lab.cpp Lab.h
	g++ -c Lab.cpp -O3

clean:
	rm *.o output