/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include <vector>
#include "ParetoSet.h"

#ifndef LAB_H
#define LAB_H

class Lab{

private:
	int K, N, M;
	std::vector<int> machineEnergy, procTime, timePrice;
	ParetoSet solutions;

public:
	Lab(const char *fileDir);

	void constructiveHeuristic();
	void printSolutions();
};

#endif