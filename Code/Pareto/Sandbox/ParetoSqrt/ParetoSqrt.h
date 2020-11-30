#include <vector>
#include "Solution.h"

#ifndef PARETOSQRT_H
#define PARETOSQRT_H

class ParetoSqrt{

private:
	std::vector<Solution> buf, ans;
	int dim;

public:

	const static int limit = 31622;

	ParetoSqrt(int _dim): dim(_dim){};
	int getDimension(){return dim;};
	int getSize();

	void insertSol(const Solution &s);
	void merge();
	void printVec();
};

#endif