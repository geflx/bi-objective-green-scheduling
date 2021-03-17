/**
 *	@author Gabriel Felix
 * 	@date	16/03/21
 */

#include "Solution.h"
#include <iostream>

using namespace std;

Solution::Solution(int N_){
	N = N_;
	routes.resize(2, vector<int> (N, -1));
}

void Solution::setObj(int x, int y){

	obj.first = x;
	obj.second = y;
}

void Solution::setV(int route, int i, int v){

	routes[route][i] = v;
}

void Solution::printSolution(){

	printf("Route_1: ");

	for(int i = 0; i < N; i++)
		printf("%d ", routes[0][i]);

	printf("Route_2: ");

	for(int i = 0; i < N; i++)
		printf("%d ", routes[1][i]);

	printf("\n");
}