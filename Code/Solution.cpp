/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include "Solution.h"
#include <iostream>

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

void Solution::printSolution(){

	printf("[");

	for(int i = 0; i < N; i++)
		if(i != (N - 1))
			printf("%d, ", setup[0][i]);
		else
			printf("%d] [", setup[0][i]);

	for(int i = 0; i < N; i++)
		if(i != (N - 1))
			printf("%d, ", setup[1][i]);
		else
			printf("%d]\n", setup[1][i]);
}