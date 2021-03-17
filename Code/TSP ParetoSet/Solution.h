/**
 *	@author Gabriel Felix
 * 	@date	16/03/21
 */

#include <vector>
#include <utility> // pair

#ifndef SOLUTION_H
#define SOLUTION_H

class Solution{

private:
	std::vector<std::vector<int>> routes;
	std::pair<int, int> obj; // (Route 1 cost, route 2 cost).
	int N;

public:
	Solution(){ N = -1;};
	Solution(int N_);

	int getN(){ return N; }
	void setObj(int x, int y);
	std::pair<int, int> getObj(){ return obj; }

	void setV(int route, int i, int v);
	void printSolution();
	int getV(int route, int i){ return routes[route][i]; }
};

#endif