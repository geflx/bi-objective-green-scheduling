#include <limits>
using namespace std;

#ifndef LOCATION_H
#define LOCATION_H

class Location{
	public:
		int machine, cost, beg, end, IsContinuous;
		Location(){
			this->machine = -1;
			this->cost = std::numeric_limits<int>::max();
			this->beg = -1;
			this->end = -1;
			this->isContinuous = true;
		}

	friend bool operator< (const Location &l1, const Location &l2);
};

bool operator< (const Location &l1, const Location &l2)
{
    return l1.cost < l2.cost;
}

#endif