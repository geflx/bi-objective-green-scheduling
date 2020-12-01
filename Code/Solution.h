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
	std::pair<int, int> obj;
	int N;

public:
	Solution(){};
	Solution(int N_);

	void setObj(int x, int y);
	std::pair<int, int> getObj(){ return obj; }

	void setV(int job, int machine, int position);
	int getV(int x, int y){ return setup[x][y]; }
};

#endif