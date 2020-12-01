#include <vector>

#ifndef PARETOVEC_H
#define PARETOVEC_H

class ParetoVec{

private:
	std::vector<std::pair<int, int>> v;
	const static int limit = 31000;

public:
	// ParetoVec();
	void insertSol(int x, int y);
	void updateVec();
	void printVec();
	
	size_t getSize();
};
#endif