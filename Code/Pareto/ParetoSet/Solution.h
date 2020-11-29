#include <vector>

class Solution{

private:
	int dim;
public:
	Solution(){};
	std::vector<int> obj;
	Solution(const std::vector<int> &obj_): obj(obj_), dim(obj_.size()){};
};