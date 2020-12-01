#include <iostream>
#include <fstream>
#include <algorithm> 
#include "ParetoSqrt.h"

using namespace std;

bool dominate(const Solution &a, const Solution &b){

	for(int i = 0; i < a.obj.size(); i++){
		if(a.obj[i] > b.obj[i]){
			return false;
		}
	}
	return true;
}

void ParetoSqrt::merge(){

	if(buf.size() == 0)
		return;

	for(int i = 0; i < ans.size(); i++)
		buf.push_back(ans[i]);

	sort(buf.begin(), buf.end(), dominate);
	
	vector<Solution> brandNew;

	Solution mini = buf[0];
	brandNew.push_back(buf[0]);

	for(int i = 1; i < buf.size(); i++){
		if(!dominate(mini, buf[i])){
			mini = buf[i];
			brandNew.push_back(mini);
		}
	}

	buf.clear();
	ans = brandNew;
}

void ParetoSqrt::insertSol(const Solution &s){
	buf.push_back(s);

	if(buf.size() >= limit)
		merge();
}

int ParetoSqrt::getSize(){
	merge();

	return ans.size();
}

void ParetoSqrt::printVec(){
	merge();

	printf("Solutions: %d\n", getSize());

	for(int i = 0; i < ans.size(); i++){
		printf("(");
		for(int j = 0; j < dim; j++){
			printf("%d", ans[i].obj[j]);
			if(j != (dim - 1))
				printf(", ");
		}
		printf(")\n");
	}
}