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

	Laboratory env("Test/manual_inst_1.txt");

	env.constructiveHeuristic();
	
	env.printSolutions();
}

