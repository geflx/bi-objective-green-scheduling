/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Lab.h"
#include <fstream>

using namespace std;

/**
 * Lab Constructor.
 * Reads instance from file directory.
 */
Lab::Lab(const char *fileDir){

	ifstream input(fileDir);

	input >> K >> N >> M;

	timePrice.resize(K);
	procTime.resize(N);
	machineEnergy.resize(M);

	for(int i = 0; i < K; i++) 
		input >> timePrice[i];

	for(int i = 0; i < N; i++) 
		input >> procTime[i];

	for(int i = 0; i < M; i++) 
		input >> machineEnergy[i];

	input.close();
}

/**
 * Print solutions.
 * Uses "ParetoSet.h" function to do this.
 */
void Lab::printSolutions(){
	solutions.printSet();
}