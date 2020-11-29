#include "ParetoSet.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <chrono> 

using namespace std;

int main(){
	ParetoSet ps;
	Solution sol;
	ps.insertSol(1, 4, sol);
	ps.insertSol(2, 1, sol);
	ps.insertSol(2, 3, sol);
	ps.insertSol(4, 2, sol);
	ps.insertSol(5, 0, sol);

	ps.printSet();
}

