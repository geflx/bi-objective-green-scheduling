/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Solution.h"

using namespace std;

Solution::Solution(int N_){
	N = N_;
	setup.resize(2, vector<int>(N, -1));
}

void Solution::setObj(int x, int y){
	obj.first = x;
	obj.second = y;
}

void Solution::setV(int job, int machine, int position){
	setup[0][job] = machine;
	setup[1][job] = position;
}