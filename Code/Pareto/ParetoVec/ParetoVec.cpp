#include "ParetoVec.h"
#include <iostream>
#include <algorithm>
#include <limits>

using namespace std;

void ParetoVec::insertSol(int x, int y){
	// Insert solution directly.
	v.push_back(make_pair(x, y));

	// If vector reaches capacity, update it.
	if(v.size() > limit)
		updateVec();
}

void ParetoVec::updateVec(){
	vector<pair<int, int>> newV;
	int minY = numeric_limits<int>::max();

	// Sort storage solutions.
	sort(v.begin(), v.end());

	// Saving Pareto solutions, discarting dominated ones.
	for(auto it: v){
		if(it.second < minY){
			minY = it.second;
			newV.push_back(it);
		}
	}
	v = newV;
}

void ParetoVec::printVec(){
	updateVec();

	printf("Solutions: %d\n", v.size());

	for(auto it : v)
		printf("(%d, %d)\n", it.first, it.second);
}

size_t ParetoVec::getSize(){
	updateVec();

	return v.size();
}
