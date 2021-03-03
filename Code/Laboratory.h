/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include <vector>
#include "ParetoSet.h"
#include "Location.h" // Auxiliar class from SimpleSplitGreedyCH.

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

	void checkSolutions();

	Location SimpleSplitGreedyCH_GetBestLocation(const vector<vector<int>> &assignmentTable, int currentK, int currentP);
	bool SimpleSplitGreedyCH_AssignLocation(vector<vector<int>> &assignmentTable, const Location &insertionLocation, const int jobId, int &objFunction);	
	Solution SimpleSplitGreedyCH_ConvertSolution(const vector<vector<int>> &assignmentTable, const int currentK, const int SolutionCost, const int SolutionMakespan);
	void 	 SimpleSplitGreedyCH();
};

#endif