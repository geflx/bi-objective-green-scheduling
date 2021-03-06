/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include <vector>
#include <utility> // pair

#ifndef SOLUTION_H
#define SOLUTION_H

class Solution{

private:
	std::vector<std::vector<int>> setup;
	std::pair<int, int> obj; // (Cost, makespan)
	int N;

public:
	Solution(){ N = -1;};
	Solution(int N_);

	int getN(){ return N; }
	void setObj(int x, int y);
	std::pair<int, int> getObj(){ return obj; }

	void setV(int job, int machine, int position);
	void printSolution();
	int getV(int x, int y){ return setup[x][y]; }
};

#endif