output: main.o ParetoVec.o
	g++ main.o ParetoVec.o -O3

main.o: main.cpp
	g++ -c main.cpp -O3

ParetoVec.o: ParetoVec.cpp ParetoVec.h
	g++ -c ParetoVec.cpp -O3

clean:
	rm *.o output