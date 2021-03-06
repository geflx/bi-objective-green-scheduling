/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "ParetoSet.h"
#include "Laboratory.h"
#include <iostream>
#include <fstream>
#include <algorithm> 
#include <chrono> 

using namespace std;

int main(){

	Laboratory env("Sandbox/RData_90.txt");

	//env.SimpleSplitGreedyCH();
	env.Fast_FenwickTreeSplitGreedyCH();
	
	env.checkSolutions();

	env.printSolutions();
}

