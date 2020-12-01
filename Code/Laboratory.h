/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include <vector>
#include "ParetoSet.h"

#ifndef LABORATORY_H
#define LABORATORY_H

class Laboratory{

private:
	int K, N, M;
	std::vector<int> machineEnergy, procTime, timePrice;
	ParetoSet solutions;

public:
	Laboratory(const char *fileDir);

	void constructiveHeuristic();
	void printSolutions();
};

#endif