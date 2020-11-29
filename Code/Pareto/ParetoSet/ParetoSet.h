#include <set>
#include <map>
#include "Solution.h"

#ifndef PARETOSET_H
#define PARETOSET_H

// @author: Gabriel Felix (geflx)

class ParetoSet{

private:
	std::set<std::pair<int, int>> s;
	std::map<std::pair<int, int>, Solution> mapSol;

public:
	ParetoSet(){};
	int getSize(){ 	return s.size();	};
	bool empty(){	return s.empty();	};

	bool isDominated(int x, int y);
	bool insertSol(int x, int y, const Solution &sol);

	void printSet();
	void removeDominated(int x, int y);
};

#endif