/**
 *	@author Gabriel Felix
 * 	@date	11/30/20
 */

#include <iostream>
#include <fstream>
#include <vector>
#include "ParetoSet.h"

using namespace std;

bool ParetoSet::isDominated(int x, int y){
	if(s.empty())
		return false;

	// Check elements starting from the lowest pair, return true if its dominated.
	auto it = s.begin();
	while(it != s.end() && it->first <= x){
		if(it->first <= x && it->second <= y)
			return true;	
		it++;
	}

	return false;
}

void ParetoSet::removeDominated(int x, int y){

	// Capture lower bound element of pair (binary search).
	auto it = s.lower_bound(make_pair(x, -1));

	// Iterate over elements and remove dominated pairs.
	while(it != s.end()){
		if(it->first >= x && it->second >= y){
			auto itTmp = it;
			it++;

			mapSol.erase(*itTmp);
			s.erase(itTmp);
		}else{
			it++;
		}
	}
}

bool ParetoSet::insertSol(int x, int y, const Solution &sol){

	// Ignore solution if it is already inserted.
	if(s.find(make_pair(x, y)) != s.end())
		return false;

	// Dont insert solution if its dominated by another one.
	if(isDominated(x, y))
		return false;
	
	// Remove dominated solutions.
	removeDominated(x, y);

	s.insert(make_pair(x, y));
	mapSol[make_pair(x, y)] = sol;
	return true;
}

void ParetoSet::printSet(){
	printf("Solutions: %d\n", getSize());

	for(auto &it: s){
		printf("(%d, %d) ", it.first, it.second + 1);

		mapSol[it].printSolution();
	}
}

vector<Solution> ParetoSet::getSolutions(){

	vector<Solution> allSolutions;

	for(auto &it: mapSol)
		allSolutions.push_back(it.second);

	return allSolutions;
}