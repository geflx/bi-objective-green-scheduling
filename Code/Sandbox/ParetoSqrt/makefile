output: main.o ParetoSqrt.o
	g++ main.o ParetoSqrt.o  -O3

main.o: main.cpp
	g++ -c main.cpp -O3

ParetoSqrt.o: ParetoSqrt.cpp ParetoSqrt.h
	g++ -c ParetoSqrt.cpp -O3

clean:
	rm *.o output